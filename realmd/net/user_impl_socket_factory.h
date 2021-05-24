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

#ifndef REALMD_NET_USER_IMPL_SOCKET_FACTORY_H_
#define REALMD_NET_USER_IMPL_SOCKET_FACTORY_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cassert>
#include "public/net/socket_factory.h"
#include "user_impl_socket.h"

class UserImplSocketFactory : public ::pear::net::SocketFactory
{
public:
    UserImplSocketFactory(void){}
    virtual ~UserImplSocketFactory(void){}

    virtual ::pear::net::Socket *Create(void)
    {
        return new UserImplSocket();
    }

    virtual void Destory(::pear::net::Socket *sock)
    {
        UserImplSocket *user_impl_sock = static_cast<UserImplSocket*>(sock);
        assert(user_impl_sock != NULL);
        delete user_impl_sock;
    }
};

#endif // REALMD_NET_USER_IMPL_SOCKET_FACTORY_H_
