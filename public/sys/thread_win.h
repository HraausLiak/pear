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

#ifndef PUBLIC_SYS_THREAD_WIN_H_
#define PUBLIC_SYS_THREAD_WIN_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>

namespace pear {
    namespace sys {

        class Thread
        {
        public:
            typedef DWORD   tid_t;

            enum State
            {
                NOT_READY,
                READY,
                RUNNING,
                EXIT
            };


            Thread(void);
            virtual ~Thread(void);

            bool Start(void);
            virtual void Stop(void) = 0;

            void Join(void);

            inline tid_t get_id(void) const { return id_; }
            inline State get_state(void) const { return state_; }

            static tid_t GetCurrentThreadId(void) { return ::GetCurrentThreadId(); }
            static void Sleep(::time_t msec) { ::Sleep((DWORD)(msec)); }

        protected:
            virtual void Run(void) = 0;

        private:
            static unsigned int __stdcall Proc(void* arg);

            tid_t id_;
            State state_;
            ::uintptr_t handle_;
        };

    }
}

#endif // PUBLIC_SYS_THREAD_WIN_H_
