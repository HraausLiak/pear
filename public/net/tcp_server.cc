/******************************************************************************
 * Copyright (C) 2015  671643387@qq.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *****************************************************************************/

#include "tcp_server.h"
#include "../sys/lock_guard.h"
#include "../sys/net.h"

#include <cassert>

#define __PEAR_LOCK(x)  ::pear::sys::LockGuard<::pear::sys::Mutex> guard(x)

namespace pear {
    namespace net {

        const static float MPA = 0.9f;

        TcpServer::TcpServer(::pear::net::SocketFactory *conn_factory)
            : listener_(main_worker_, &TcpServer::OnAccept, this)
            , conn_id_gen_(0)
            , connections_()
            , conn_lock_()
            , conn_factory_(conn_factory)
            , main_worker_()
            , addtional_workers_num_(0)
            , addtional_workers_(NULL)
            , errno_(0)
            , error_message_("")
        {
            assert(conn_factory);
        }

        TcpServer::~TcpServer(void)
        {

        }

        bool TcpServer::Start(const char *host, int port, int backlog, int addtional_workers)
        {
            addtional_workers_num_ = addtional_workers;

            do 
            {
                // start main worker, must before start listener because listener used the main worker
                if (!main_worker_.Start()) {
                    errno_ = errno;
                    break;
                }

                // start listener
                if (!listener_.Open(host, port, backlog)) {
                    errno_ = ::pear::sys::GetNetErrorCode();
                    ::pear::sys::GetNetErrorMessage(errno_, error_message_);
                    break;
                }

                // create addtional workers
                if (addtional_workers > 0 && !CreateAddtionalWorkers(addtional_workers))
                {
                    errno_ = errno;
                    break;
                }

                return true;
            } while (false);

            CloseAddtionalWorkers();

            main_worker_.Stop();
            main_worker_.Join();

            listener_.Close();

            return false;
        }

        void TcpServer::Stop(void)
        {
            // stop listener
            listener_.Close();

            // stop addtional workers
            CloseAddtionalWorkers();

            // stop main worker
            main_worker_.Stop();
            main_worker_.Join();

            // close all connections
            {
                __PEAR_LOCK(conn_lock_);
                SocketMap::iterator iter = connections_.begin();
                for (; iter != connections_.end(); ++iter)
                {
                    conn_factory_->Destory(iter->second);
                }
                connections_.clear();
            }
        }

        void TcpServer::Join(void)
        {
            // join addtional workers
            if (addtional_workers_num_ > 0)
            {
                for (int x = 0; x < addtional_workers_num_; ++x) {
                    addtional_workers_[x]->Join();
                }
            }

            // join main worker
            main_worker_.Join();
        }

        void TcpServer::CloseSocket(Socket *sock)
        {
            assert(sock != NULL);
            {
                __PEAR_LOCK(conn_lock_);
                SocketMap::iterator iter = connections_.find(sock->get_id());
                assert(iter != connections_.end());
                if (iter != connections_.end())
                {
                    connections_.erase(iter);
                }
            }
            conn_factory_->Destory(sock);
        }

        bool TcpServer::CreateAddtionalWorkers(int addtional_workers)
        {
            do 
            {
                addtional_workers_ = new ::pear::net::Thread*[MAX_WORKERS];
                if (!addtional_workers_) {
                    return false;
                }
                memset(addtional_workers_, 0, sizeof(::pear::net::Thread*) * MAX_WORKERS);

                int num_workers = 0;
                for (; num_workers < addtional_workers; ++num_workers)
                {
                    ::pear::net::Thread *worker = new ::pear::net::Thread();
                    if (!worker) {
                        return false;
                    }

                    if (worker->Start()) {
                        addtional_workers_[num_workers] = worker;
                        continue;
                    }

                    delete worker;
                }
            } while (false);

            return true;
        }

        void TcpServer::CloseAddtionalWorkers(void)
        {
            if (addtional_workers_num_ > 0 && addtional_workers_)
            {
                for (int x = 0; x < addtional_workers_num_; ++x)
                {
                    if (addtional_workers_[x] != NULL)
                    {
                        addtional_workers_[x]->Stop();
                        addtional_workers_[x]->Join();
                        delete addtional_workers_[x];
                        addtional_workers_[x] = NULL;
                    }
                }

                delete[] addtional_workers_;
            }
        }

        void TcpServer::OnAccept(::evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
        {
            TcpServer *tcp_server = static_cast<TcpServer*>(user_data);
            assert(tcp_server);
            tcp_server->DoAccept(fd, sa, socklen, user_data);
        }

        void TcpServer::DoAccept(::evutil_socket_t fd, struct sockaddr * sa, int socklen, void *)
        {
            do 
            {
                // find a connection id
                int conn_id = ++conn_id_gen_;
                {
                    __PEAR_LOCK(conn_lock_);
                    if (connections_.find(conn_id) != connections_.end()) {
                        break;
                    }
                }

                // accept the connection
                Socket *sock = conn_factory_->Create();
                if (sock)
                {
                    ::pear::net::Thread *worker = FindWorker();
                    if (sock->Open(worker->AcquirEventBase(), fd, sa, socklen))
                    {
                        sock->set_id(conn_id);
                        {
                            __PEAR_LOCK(conn_lock_);
                            connections_[conn_id] = sock;
                        }
                        sock->OnOpen();
                        return;
                    }
                    conn_factory_->Destory(sock);
                }

                // release the connection id
                --conn_id_gen_;
            } while (false);

            ::pear::sys::CloseSocket(fd);
        }

        ::pear::net::Thread *TcpServer::FindWorker(void)
        {
            if (addtional_workers_num_ > 0)
            {
                int worker_idx = 0;
                int min_load = INT_MAX;

                // find most free attional worker
                for (int x = 0; x < addtional_workers_num_; ++x)
                {
                    int worker_load = addtional_workers_[x]->get_busy_load();
                    if (worker_load < min_load)
                    {
                        min_load = worker_load;
                        worker_idx = x;
                    }
                }

                int main_load = (int)(main_worker_.get_busy_load() * MPA);
                if (main_load > min_load) {
                    return addtional_workers_[worker_idx];
                }
            }

            return &main_worker_;
        }
    }
}