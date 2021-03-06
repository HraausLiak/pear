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

#include "log.h"
#include "../sys/process.h"
#include "../sys/thread.h"
#include "../util/string.h"
#include "../util/time.h"

#include <cassert>
#include <cerrno>
#include <ctime>
#include <cstdio>
#include <cstring>

#define LOG_MSG_SIZE    0x1000

namespace pear {
    namespace log {

        static const char *SeverityLevelStrings[] = 
        { 
            "DEBUG",
            "INFO",
            "LOG",
            "WARNING",
            "ERROR",
        };

        Log::Log(void)
            : inited_(false)
            , file_id_(0)
            , console_(CON_NUL)
            , rotate_(ROT_FILE_DATE)
            , rotate_file_size_(0x8000000) // 128MB
            , flush_file_size_(0x1000)
            , directory_(NULL)
            , file_name_(NULL)
            , severity_level_(SL_DEBUG)

            , log_file_()
            , log_size_(0)
            , log_write_size_(0)
        {
            memset(&log_file_tm_, 0, sizeof(struct tm));
        }

        Log::~Log(void)
        {
        
        }

        int Log::Setup(int console, int rotate, const char *directory, const char *filename /* = "" */)
        {
            assert(!inited_);
            console_ = console;
            rotate_ = rotate;
            directory_ = directory;
            file_name_ = filename;

            return 0;
        }

        void Log::info(const char* format, ...)
        {
            if (SL_INFO < severity_level_) {
                return;
            }

            ::std::va_list args;
            va_start(args, format);
            PrintMessage(format, args, SL_INFO);
            va_end(args);
        }

        void Log::log(const char* format, ...)
        {
            if (SL_LOG < severity_level_) {
                return;
            }

            ::std::va_list args;
            va_start(args, format);
            PrintMessage(format, args, SL_LOG);
            va_end(args);
        }

        void Log::warning(const char* format, ...)
        {
            if (SL_WARNING < severity_level_) {
                return;
            }

            ::std::va_list args;
            va_start(args, format);
            PrintMessage(format, args, SL_WARNING);
            va_end(args);
        }

        void Log::error(const char* format, ...)
        {
            if (SL_ERROR < severity_level_) {
                return;
            }

            ::std::va_list args;
            va_start(args, format);
            PrintMessage(format, args, SL_ERROR);
            va_end(args);
        }

        void Log::debug(const char* format, ...)
        {
            if (SL_DEBUG < severity_level_) {
                return;
            }

            ::std::va_list args;
            va_start(args, format);
            PrintMessage(format, args, SL_DEBUG);
            va_end(args);
        }

        const char *Log::FindFile(char *filename, int size)
        {
            assert(filename && size > 0);

            const char *modname = ::pear::sys::GetModuleName();
            time_t now = time(NULL);
            struct tm local_time = { 0 };
            ::pear::util::Time::localtime(&local_time, &now);

            char errmsg[0x200];

            // test origin file
            int str_len = ::pear::util::String::snprintf(filename, size, "%s%s%s.%04d%02d%02d.log", directory_, modname, file_name_,
                local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday);
            if (str_len < 0) {
                printf("error: %s,%d -- %s\n", __FILE__, __LINE__,
                    strerror_s(errmsg, sizeof(errmsg), errno));
                return NULL;
            }

            if (!::pear::sys::File::Exists(filename)) {
                return filename;
            }

            int id = 1;
            for (;; ++id)
            {
                // test id file
                str_len = ::pear::util::String::snprintf(filename, size, "%s%s%s.%04d%02d%02d.%d.log", directory_, modname, file_name_,
                    local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday, id);
                if (str_len < 0) {
                    printf("error: %s,%d -- %s\n", __FILE__, __LINE__,
                        strerror_s(errmsg, sizeof(errmsg), errno));
                    return NULL;
                }
                if (!::pear::sys::File::Exists(filename)) {
                    break;
                }
            }

            --id;
            if (id < 1) {
                if (::pear::util::String::snprintf(filename, size, "%s%s%s.%04d%02d%02d.log", directory_, modname, file_name_,
                    local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday) < 0) {
                    printf("error: %s,%d -- %s\n", __FILE__, __LINE__,
                        strerror_s(errmsg, sizeof(errmsg), errno));
                    return NULL;
                }
            }
            else {
                file_id_ = id;
                if (::pear::util::String::snprintf(filename, size, "%s%s%s.%04d%02d%02d.%d.log", directory_, modname, file_name_,
                    local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday, id) < 0) {
                    printf("error: %s,%d -- %s\n", __FILE__, __LINE__,
                        strerror_s(errmsg, sizeof(errmsg), errno));
                    return NULL;
                }
            }

            return filename;
        }

        const char *Log::GenFileName(char *filename, int size, struct tm *time, bool rot_size /* = true */)
        {
            char errmsg[0x200];
            const char *modname = ::pear::sys::GetModuleName();

            if (rot_size)
            {
                ++file_id_;
                if (::pear::util::String::snprintf(filename, size, "%s%s%s.%04d%02d%02d.%d.log", directory_, modname, file_name_,
                    time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, file_id_) < 0) {
                    --file_id_;
                    printf("error: %s,%d -- %s\n", __FILE__, __LINE__,
                        strerror_s(errmsg, sizeof(errmsg), errno));
                    return NULL;
                }
            }
            else
            {
                if (::pear::util::String::snprintf(filename, size, "%s%s%s.%04d%02d%02d.log", directory_, modname, file_name_,
                    time->tm_year + 1900, time->tm_mon + 1, time->tm_mday) < 0) {
                    printf("error: %s,%d -- %s\n", __FILE__, __LINE__,
                        strerror_s(errmsg, sizeof(errmsg), errno));
                    return NULL;
                }
            }

            return filename;
        }

        void Log::PrintMessage(const char *format, ::std::va_list args, SeverityLevel severity_level)
        {
            assert(LOG_MSG_SIZE >= 0x200);

            char errmsg[0x200];
            int message_size = 0;
            char message[LOG_MSG_SIZE];

            // message head
            time_t now = time(NULL);
            struct tm local_time = { 0 };
            ::pear::util::Time::localtime(&local_time, &now);
            int str_len = ::pear::util::String::snprintf(message, LOG_MSG_SIZE, "[%04d-%02d-%02d %02d:%02d:%02d], %s, %d, ",
                local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday,
                local_time.tm_hour, local_time.tm_min, local_time.tm_sec, SeverityLevelStrings[severity_level], 
                ::pear::sys::Thread::GetCurrentThreadId());
            if (str_len < 0) {
                printf("error: %s,%d -- %s\n", __FILE__, __LINE__,
                    strerror_s(errmsg, sizeof(errmsg), errno));
                return;
            }
            message_size += str_len;

            // message content
            str_len = ::pear::util::String::vsnprintf(&message[str_len], LOG_MSG_SIZE - str_len, format, args);
            if (str_len < 0) {
                printf("error: %s,%d -- %s\n", __FILE__, __LINE__,
                    strerror_s(errmsg, sizeof(errmsg), errno));
                return;
            }
            message_size += str_len;

            if (message_size < LOG_MSG_SIZE) {
                message[message_size] = '\n';
                message[++message_size] = '\0';
            }
            else {
                message[message_size - 1] = '\n';
            }

            // print message to standard console
            if (console_ & CON_OUT) {
                fprintf(stdout, message);
            } else if (console_ & CON_ERR) {
                fprintf(stderr, message);
            }

            // write message to file
            WriteMessage(message, message_size, &local_time);
        }

        void Log::WriteMessage(const char *message, int message_size, struct tm *time)
        {
            if (!log_file_.IsValid())
            {
                // open log file
                char filename[MAX_PATH];
                if (FindFile(filename, MAX_PATH) != NULL) {
                    if (0 == log_file_.Open(filename)) {
                        log_size_ = log_file_.GetFileSize();
                        log_write_size_ = 0;
                        log_file_.GetTimes(&log_file_tm_, NULL, NULL);
                    }
                }
            }

            if (log_file_.IsValid())
            {
                char filename[MAX_PATH];
                // rotate log file
                if ((rotate_ & ROT_FILE_SIZE) && 
                    ((message_size + log_size_) > rotate_file_size_)) {
                    if (GenFileName(filename, MAX_PATH, time) != NULL) {
                        log_file_.Close();
                        if (0 == log_file_.Open(filename)) {
                            log_size_ = log_file_.GetFileSize();
                            log_write_size_ = 0;
                            log_file_.GetTimes(&log_file_tm_, NULL, NULL);
                        }
                    }
                }
                if ((rotate_ & ROT_FILE_DATE) && 
                    (log_file_tm_.tm_year != time->tm_year 
                    || log_file_tm_.tm_mon != time->tm_mon 
                    || log_file_tm_.tm_mday != time->tm_mday)) {
                    if (GenFileName(filename, MAX_PATH, time, false) != NULL) {
                        log_file_.Close();
                        if (0 == log_file_.Open(filename)) {
                            log_size_ = log_file_.GetFileSize();
                            log_write_size_ = 0;
                            log_file_.GetTimes(&log_file_tm_, NULL, NULL);
                        }
                    }
                }
            }

            if (log_file_.IsValid())
            {
                // write log
                ::std::size_t bytes_writen = log_file_.Write(
                    (const ::std::uint8_t*)message, message_size);

                log_size_ += bytes_writen;
                log_write_size_ += bytes_writen;
                if (log_write_size_ > flush_file_size_)
                {
                    log_file_.Flush();
                    log_write_size_ = 0;
                }
            }
        }
    }
}