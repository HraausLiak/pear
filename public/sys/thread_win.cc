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

#include "thread_win.h"
#include <cassert>

namespace pear {
    namespace sys {

        Thread::Thread(void)
            : id_(0)
            , state_(NOT_READY)
            , handle_(-1L)
        {

        }

        Thread::~Thread(void)
        {
            if (handle_ != -1L) {
                ::CloseHandle((HANDLE)handle_);
            }
        }

        bool Thread::Start(void)
        {
            assert(state_ == NOT_READY);
            handle_ = ::_beginthreadex(NULL, 0, Thread::Proc,
                this, 0, (unsigned int*)&id_);
            if (handle_ != -1L) {
                state_ = READY;
                return true;
            }
            return false;
        }

        void Thread::Join(void)
        {
            if (handle_ != -1L)
            {
                ::WaitForSingleObject(HANDLE(handle_), INFINITE);
                ::CloseHandle(HANDLE(handle_));
                handle_ = -1L;
            }
        }

        unsigned int Thread::Proc(void* arg)
        {
            assert(arg);
            Thread* pth = static_cast<Thread*>(arg);
            pth->state_ = RUNNING;
            pth->Run();
            pth->state_ = EXIT;
            return 0;
        }

    }
}