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

#include "string.h"

#include <cstdio>
#include <cstdarg>
#include <cstring>

namespace pear {
    namespace util {

        int String::snprintf(char *dest, int size, char *format, ...)
        {
            int rc = -1;
            ::std::va_list args;
            va_start(args, format);
            rc = String::vsnprintf(dest, size, format, args);
            va_end(args);
            return rc;
        }

        int String::vsnprintf(char *dest, int size, const char *format, ::std::va_list args)
        {
#ifdef __WINDOWS__
            return ::vsnprintf_s(dest, size, _TRUNCATE, format, args);
#else
#error
#endif
        }

        const char *String::strerror(char *dest, int size, int eno)
        {
#ifdef __WINDOWS__
            ::strerror_s(dest, size, eno);
#else
#error
#endif
            return dest;
        }
    }
}