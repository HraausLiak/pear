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

#include "log_core.h"
#include "log.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

namespace pear {
    namespace log {

        LogCore *LogCore::core_ = NULL;

        LogCore::LogCore(void)
            : log_index_(0)
        {
            assert(core_ == NULL);
            memset(logs_, 0, sizeof(logs_));

            if (core_ != NULL) {
                exit(-1);
            }
            core_ = this;
        }

        LogCore::~LogCore(void)
        {

        }

        Log *LogCore::NewLog(void)
        {
            if (log_index_ >= MAX_LOGS) {
                return NULL;
            }

            Log *log = new Log();
            logs_[++log_index_] = log;
            return log;
        }

        void LogCore::DeleteLog(Log *log)
        {
            int i = 0;
            for (; i < MAX_LOGS; ++i)
            {
                if (logs_[i] == log)
                {
                    break;
                }
            }

            delete log;

            if (i < MAX_LOGS)
            {
                logs_[i] = NULL;
            }
            else {
                fprintf(stderr, "this Log is not create by the LogCore\n");
            }
        }
    }
}