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

#ifndef PUBLIC_NET_SERVER_SOCKET_H_
#define PUBLIC_NET_SERVER_SOCKET_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <event2/event.h>
#include <event2/listener.h>

#include "public/net/server_socket_events.h"
#include "public/net/thread.h"

namespace pear {
    namespace net {

        class ServerSocket
        {
        public:
            ServerSocket(::pear::net::Thread &runnable, ::pear::net::EVENT_LISTEN on_listen = NULL, void *user_data = NULL);
            ~ServerSocket(void);

            bool Open(const char *address, int port, int backlog);
            void Close(void);

            inline void set_listen_callback(::pear::net::EVENT_LISTEN callback) {
                listen_callback_ = callback;
            }
            inline void set_user_data(void *user_data) {
                user_data_ = user_data;
            }

        private:
            static void AcceptCallback(
                struct evconnlistener *listener,
                evutil_socket_t fd,
                struct sockaddr *sa,
                int socklen,
                void *user_data
                );

            
            ::pear::net::Thread &runnable_;
            struct evconnlistener* acceptor_;

            // events
            void *user_data_;
            ::pear::net::EVENT_LISTEN listen_callback_;
        };

    }
}

#endif // PUBLIC_NET_SERVER_SOCKET_H_
