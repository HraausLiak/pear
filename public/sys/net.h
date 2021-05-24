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

#ifndef PUBLIC_SYS_NET_H_
#define PUBLIC_SYS_NET_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <string>

#ifdef __WINDOWS__
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#error
#endif

#include <event2/util.h>

namespace pear {
    namespace sys {

#ifdef __WINDOWS__
        inline void CloseSocket(evutil_socket_t fd) {
            closesocket((SOCKET)fd);
        }

        inline int GetNetErrorCode(void) {
            return ::WSAGetLastError();
        }

        inline ::std::string& GetNetErrorMessage(int code, ::std::string& message) {
            char *s = NULL;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, code,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR)&s, 0, NULL);
            message = s;
            LocalFree(s);
            return message;
        }
#else
#error
#endif


    }
}

#endif // PUBLIC_SYS_NET_H_
