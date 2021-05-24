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

#ifndef PUBLIC_NET_TCP_SERVER_H_
#define PUBLIC_NET_TCP_SERVER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>

#include "public/net/socket.h"
#include "public/net/server_socket.h"
#include "public/net/thread.h"
#include "public/net/socket_factory.h"
#include "public/sys/mutex.h"

namespace pear {
    namespace net {

        class TcpServer
        {
        public:
            const static int MAX_WORKERS = 0x80;

            TcpServer(::pear::net::SocketFactory *conn_factory);
            ~TcpServer(void);

            bool Start(const char *host, int port, int backlog, int addtional_workers = 0);
            void Stop(void);
            void Join(void);

            void CloseSocket(Socket *sock);

            inline int get_errno(void) const {
                return errno_;
            }

            inline const ::std::string& get_error_message(void) const {
                return error_message_;
            }

        private:
            bool CreateAddtionalWorkers(int addtional_workers);
            void CloseAddtionalWorkers(void);

            static void OnAccept(::evutil_socket_t, struct sockaddr *, int, void *);
            void DoAccept(::evutil_socket_t, struct sockaddr *, int, void *);

            ::pear::net::Thread *FindWorker(void);

            ::pear::net::ServerSocket listener_;

            // clients
            typedef ::boost::unordered_map<int, ::pear::net::Socket*>   SocketMap;
            volatile int conn_id_gen_;
            SocketMap connections_;
            ::pear::sys::Mutex conn_lock_;
            ::boost::scoped_ptr<::pear::net::SocketFactory> conn_factory_;

            // main worker
            ::pear::net::Thread main_worker_;
            // addtional workers
            int addtional_workers_num_;
            ::pear::net::Thread **addtional_workers_;

            // error
            int errno_;
            ::std::string error_message_;
        };

    }
}

#endif // PUBLIC_NET_TCP_SERVER_H_
