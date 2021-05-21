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

#include "file.h"

#include <cerrno>

namespace pear {
    namespace util {

#ifdef __WINDOWS__
        File::File(void)
            : handle_(INVALID_HANDLE_VALUE)
        {

        }

        File::~File(void)
        {
            Close();
        }

        int File::Open(const char *filename, AccessMode access_mode /* = AM_RW */)
        {
            DWORD dwAccess = 0;
            if (access_mode & AM_READ) {
                dwAccess |= GENERIC_READ;
            }
            if (access_mode & AM_WRITE) {
                dwAccess = GENERIC_WRITE;
            }
            handle_ = ::CreateFile(filename, dwAccess, FILE_SHARE_READ,
                NULL, OPEN_ALWAYS, 0, NULL);
            return errno;
        }

        void File::Close(void)
        {
            if (handle_ != INVALID_HANDLE_VALUE) {
                ::FlushFileBuffers(handle_);
                ::CloseHandle(handle_);
            }
        }

        long int File::GetFileSize(void)
        {
            return (long int)::GetFileSize(handle_, NULL);
        }

        void File::GetTimes(struct tm *ctime, struct tm *mtime, struct tm *atime)
        {
            FILETIME c_time, m_time, a_time, l_time;
            SYSTEMTIME s_time;
            if (::GetFileTime(handle_, &c_time, &a_time, &m_time))
            {
                if (ctime != NULL && ::FileTimeToLocalFileTime(&c_time, &l_time)) {
                    ::FileTimeToSystemTime(&l_time, &s_time);
                    ctime->tm_year = s_time.wYear - 1900;
                    ctime->tm_mon = s_time.wMonth - 1;
                    ctime->tm_mday = s_time.wDay;
                    ctime->tm_hour = s_time.wHour;
                    ctime->tm_min = s_time.wMinute;
                    ctime->tm_sec = s_time.wSecond;
                }
                if (mtime != NULL && ::FileTimeToLocalFileTime(&m_time, &l_time)) {
                    ::FileTimeToSystemTime(&l_time, &s_time);
                    mtime->tm_year = s_time.wYear - 1900;
                    mtime->tm_mon = s_time.wMonth - 1;
                    mtime->tm_mday = s_time.wDay;
                    mtime->tm_hour = s_time.wHour;
                    mtime->tm_min = s_time.wMinute;
                    mtime->tm_sec = s_time.wSecond;
                }
                if (atime != NULL && ::FileTimeToLocalFileTime(&a_time, &l_time)) {
                    ::FileTimeToSystemTime(&l_time, &s_time);
                    atime->tm_year = s_time.wYear - 1900;
                    atime->tm_mon = s_time.wMonth - 1;
                    atime->tm_mday = s_time.wDay;
                    atime->tm_hour = s_time.wHour;
                    atime->tm_min = s_time.wMinute;
                    atime->tm_sec = s_time.wSecond;
                }
            }
        }

        ::std::size_t File::Write(const char *data, ::std::size_t size)
        {
            DWORD bytes_writen = 0;
            ::WriteFile(handle_, data, size, &bytes_writen, NULL);
            return (::std::size_t)bytes_writen;
        }

        void File::Flush(void)
        {
            ::FlushFileBuffers(handle_);
        }

        bool File::Exists(const char *filename) {
            WIN32_FIND_DATA find_file_data;
            HANDLE find_handle = ::FindFirstFile(filename, &find_file_data);
            if (find_handle == INVALID_HANDLE_VALUE) {
                return false;
            }
            return true;
        }
#endif

    }
}