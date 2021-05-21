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

namespace pear {
    namespace net {

        TcpServer::TcpServer(::pear::net::SocketFactory *conn_factory)
            : listener_(main_worker_)
            , conn_id_gen_(0)
            , connections_()
            , conn_factory_(conn_factory)
            , main_worker_()
        {

        }

        TcpServer::~TcpServer(void)
        {

        }

        bool TcpServer::Start(const char *host, int port, int backlog, int addtional_workers)
        {
            do 
            {
                // start listener
                if (!listener_.Open(host, port, backlog)) {
                    break;
                }

                // start main worker
                if (!main_worker_.Start()) {
                    break;
                }

                // create addtional workers
                if (addtional_workers > 0 && !CreateAddtionalWorkers(addtional_workers))
                {
                    break;
                }

                return true;
            } while (false);

            CloseAddtionalWorkers();
            main_worker_.Stop();
            listener_.Close();

            return false;
        }

        void TcpServer::Stop(void)
        {

        }

        bool TcpServer::CreateAddtionalWorkers(int addtional_workers)
        {
            do 
            {
                addtional_workers_ = new ::pear::net::Thread*[MAX_WORKERS];
                if (!addtional_workers_) {
                    return false;
                }

                int num_workers = 0;
                for (; num_workers < addtional_workers; ++num_workers)
                {
                    ::pear::net::Thread *worker = new ::pear::net::Thread();
                    if (!worker) {
                        return false;
                    }

                    if (worker->Start()) {
                        addtional_workers_[num_workers] = worker;
                        continue;;
                    }

                    delete worker;
                }
            } while (false);

            return true;
        }

        void TcpServer::CloseAddtionalWorkers(void)
        {

        }

        void TcpServer::OnAccept(::evutil_socket_t fd, struct sockaddr *sa, int, void *socklen)
        {

        }

    }
}