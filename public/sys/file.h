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

#ifndef PUBLIC_SYS_FILE_H_
#define PUBLIC_SYS_FILE_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstdint>
#include <ctime>
#include <cstdio>

#ifdef __WINDOWS__
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace pear {
    namespace sys {

#ifdef __WINDOWS__
        class File
        {
        public:
            const static int ACCESS_READ = 0x1;
            const static int ACCESS_WRITE = 0x2;

            const static int FILE_NO_BUFFERING = 0x1;

            File(void);
            virtual ~File(void);

            int Open(const char *filename, int access_mode = ACCESS_READ | ACCESS_WRITE, int flags = 0);
            void Close(void);

            long int GetFileSize(void);
            bool GetTimes(struct tm *ctime, struct tm *mtime, struct tm *atime);

            ::std::size_t Read(::std::uint8_t *data, ::std::size_t size);
            ::std::size_t Write(const ::std::uint8_t *data, ::std::size_t size);
            bool Flush(void);

            bool IsValid(void) {
                return handle_ != INVALID_HANDLE_VALUE;
            }

            static bool Exists(const char *filename);

        private:
            HANDLE handle_;
        };
#endif
    }
}

#endif // PUBLIC_SYS_FILE_H_
