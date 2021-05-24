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

#ifndef REALMD_NET_USER_IMPL_SOCKET_H_
#define REALMD_NET_USER_IMPL_SOCKET_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include "public/net/socket.h"

class UserImplSocket : public ::pear::net::Socket
{
public:
    UserImplSocket(void);
    virtual ~UserImplSocket(void);

protected:
    virtual void OnOpen(void);
    virtual void OnRead(void);
    virtual void OnWrite(void);
    virtual void OnEof(void);
    virtual void OnClose(void);
    virtual void OnError(void);
};

#endif // REALMD_NET_USER_IMPL_SOCKET_H_
