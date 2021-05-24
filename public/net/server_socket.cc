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

#include "server_socket.h"

#include <cassert>
#include <cstring>
#include <event2/listener.h>

namespace pear {
    namespace net {

        ServerSocket::ServerSocket(::pear::net::Thread &runnable, 
            ::pear::net::EVENT_LISTEN on_listen /* = NULL */, void *user_data /* = NULL */)
            : runnable_(runnable)
            , acceptor_(NULL)

            , user_data_(user_data)
            , listen_callback_(on_listen)
        {

        }

        ServerSocket::~ServerSocket(void)
        {
            Close();
        }

        bool ServerSocket::Open(const char *address, int port, int backlog)
        {
            struct sockaddr_in saddr;
            ::memset(&saddr, 0, sizeof(saddr));
            saddr.sin_family = AF_INET;
#if defined(__WINDOWS__)
            saddr.sin_addr.S_un.S_addr = ::inet_addr(address);
#elif defined(__LINUX__)
            saddr.sin_addr.s_addr = ::inet_addr(address);
#endif
            saddr.sin_port = ::htons(u_short(port));

            acceptor_ = ::evconnlistener_new_bind(
                runnable_.AcquirEventBase(), &ServerSocket::AcceptCallback, this,
                LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, backlog,
                (struct sockaddr*)&saddr,
                sizeof(saddr));
            if (!acceptor_) {
                runnable_.ReleaseEventBase();
                return false;
            }
            return true;
        }

        void ServerSocket::Close(void)
        {
            if (acceptor_ != NULL) {
                ::evconnlistener_free(acceptor_);
                acceptor_ = NULL;
            }
        }

        void ServerSocket::AcceptCallback(
            struct evconnlistener *listener, 
            evutil_socket_t fd, struct sockaddr *sa, 
            int socklen, void *user_data)
        {
            ServerSocket *pss = static_cast<ServerSocket*>(user_data);
            assert(pss && pss->listen_callback_);
            pss->listen_callback_(fd, sa, socklen, pss->user_data_);
        }

    }
}