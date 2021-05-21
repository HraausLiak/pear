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

#include "socket.h"
#include "public/util/string.h"

#include <cerrno>
#include <cassert>
#include <event2/event.h>

namespace pear {
    namespace net {

        Socket::Socket(void)
            : fd_(-1)
            , bev_(NULL)
            , host_("")
            , port_(0)
            , error_string_("")
            , errno_(0)
            , fatal_error_(false)
        {

        }

        Socket::~Socket(void)
        {
            Close();
        }

        bool Socket::Open(struct event_base *ev_base, ::evutil_socket_t fd, 
            struct sockaddr *sa /* = NULL */, int socklen /* = 0 */)
        {
            assert(ev_base && fd != -1);
            fd_ = fd;

            do 
            {
                if (sa && socklen) {
                    struct sockaddr_in* addr = (struct sockaddr_in*)(sa);
                    host_ = std::string(::inet_ntoa(addr->sin_addr));
                    port_ = int(::ntohs(addr->sin_port));
                }

                bev_ = bufferevent_socket_new(ev_base, fd, BEV_OPT_CLOSE_ON_FREE);
                if (!bev_) {
                    break;
                }

                bufferevent_setcb(bev_, 
                    &Socket::ReadCallback, &Socket::WriteCallback, &Socket::EventCallback, this);
                bufferevent_disable(bev_, EV_WRITE);
                bufferevent_enable(bev_, EV_READ);

                return true;
            } while (false);

            return false;
        }

        void Socket::Close(void)
        {
            if (bev_)
            {
                bufferevent_disable(bev_, EV_READ);
                bufferevent_free(bev_);
                bev_ = NULL;
                fd_ = -1;
            }
        }

        void Socket::Send(const ::std::uint8_t *data, ::std::size_t size)
        {
            if (!fatal_error_)
            {
                bufferevent_enable(bev_, EV_WRITE);
                if (bufferevent_write(bev_, data, size) != 0) {
                    EofOrError();
                }
            }
        }

        void Socket::EofOrError(void)
        {
            if (fatal_error_) {
                return;
            }
            fatal_error_ = true;

            bufferevent_disable(bev_, EV_WRITE);
        }

        void Socket::ReadCallback(struct bufferevent *, void *user_data)
        {
            Socket *psock = static_cast<Socket*>(user_data);
            assert(psock);
            psock->OnRead();
        }

        void Socket::WriteCallback(struct bufferevent *, void *user_data)
        {
            Socket *psock = static_cast<Socket*>(user_data);
            assert(psock);
            
            if (psock->fatal_error_)
            {
                bufferevent_disable(psock->bev_, EV_WRITE);
                psock->OnWrite();
            }
        }

        void Socket::EventCallback(struct bufferevent *, short events, void *user_data)
        {
            Socket *psock = static_cast<Socket*>(user_data);
            assert(psock);

            if (events & BEV_EVENT_EOF) {
                psock->EofOrError();
                psock->OnEof();
            }
            else if (events & BEV_EVENT_ERROR) {
                char errbuf[0x100];
                psock->errno_ = errno;
                psock->error_string_ = ::pear::util::String::strerror(errbuf, sizeof(errbuf), errno);
                psock->EofOrError();
                psock->OnError();
            }
            else if (events & BEV_EVENT_TIMEOUT) {
                psock->OnTimeout();
            }
        }
    }
}