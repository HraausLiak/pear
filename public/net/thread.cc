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

#include "thread.h"

#include <cassert>

namespace pear {
    namespace net {

        Thread::Thread(void)
            : ev_base_(NULL)
            , busy_load_(0)
        {

        }

        Thread::~Thread(void)
        {
            if (ev_base_ != NULL) {
                ::event_base_free(ev_base_);
            }
        }

        bool Thread::Start(void)
        {
            do 
            {
                if ((ev_base_ = event_base_new()) == NULL) {
                    break;
                }
                return ::pear::sys::Thread::Start();
            } while (false);
            return false;
        }

        void Thread::Stop(void)
        {
            assert(ev_base_ != NULL);
            ::event_base_loopbreak(ev_base_);
        }

        void Thread::Run(void)
        {
            assert(ev_base_ != NULL);
            ::event_base_dispatch(ev_base_);
        }
    }
}