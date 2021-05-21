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

#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>

#include "public/net/socket.h"
#include "public/net/server_socket.h"
#include "public/net/thread.h"
#include "public/net/socket_factory.h"

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

        private:
            bool CreateAddtionalWorkers(int addtional_workers);
            void CloseAddtionalWorkers(void);

            static void OnAccept(::evutil_socket_t, struct sockaddr *, int, void *);

            ::pear::net::ServerSocket listener_;

            // clients
            typedef ::boost::unordered_map<int, ::pear::net::Socket*>   SocketMap;
            int conn_id_gen_;
            SocketMap connections_;
            ::boost::scoped_ptr<::pear::net::SocketFactory> conn_factory_;

            // main worker
            ::pear::net::Thread main_worker_;
            // addtional workers
            ::pear::net::Thread **addtional_workers_;
        };

    }
}

#endif // PUBLIC_NET_TCP_SERVER_H_
