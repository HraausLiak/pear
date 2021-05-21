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

#ifndef PUBLIC_LOG_LOG_H_
#define PUBLIC_LOG_LOG_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstdarg>
#include <cstdio>

#include "public/log/log_file.h"

namespace pear {
    namespace log {
        class LogCore;

        class Log
        {
        public:
            enum Console
            {
                CON_OUT = 0x1,
                CON_ERR = 0x2,
                CON_NUL = 0x4
            };

            enum Rotate
            {
                ROT_FILE_SIZE = 0x1,
                ROT_FILE_DATE = 0x2
            };

            enum SeverityLevel
            {
                SL_DEBUG = 0x00,
                SL_INFO = 0x01,
                SL_LOG = 0x02,
                SL_WARNING = 0x03,
                SL_ERROR = 0x04
            };

        protected:
            friend class LogCore;
            Log(void);
            ~Log(void);

        public:

            int Setup(int console, int rotate, const char *directory);

            void set_rotate_size(int size) {
                rotate_file_size_ = size;
            }

            void info(const char* format, ...);
            void log(const char* format, ...);
            void warning(const char* format, ...);
            void error(const char* format, ...);
            void debug(const char* format, ...);

        private:
            const char *FindFile(char *filename, int size); // find the lastest log file
            const char *GenFileName(char *filename, int size, struct tm *time, bool rot_size = true);
            void PrintMessage(const char *format, ::std::va_list args, SeverityLevel severity_level);
            void WriteMessage(const char *message, int message_size, struct tm *time);

            bool inited_;

            int file_id_;

            int console_;
            int rotate_;
            int rotate_file_size_;
            int flush_file_size_;
            const char *directory_;
            SeverityLevel severity_level_;

            //// current log file
            ::pear::log::LogFile log_file_;     // log file
            long int log_size_;                 // log file size
            int log_write_size_;                // log file current write size
            struct tm log_file_tm_;             // log file create time
        };

    }
}

#endif // PUBLIC_LOG_LOG_H_
