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
            , ev_dummy_(NULL)
            , busy_load_(0)
            , stopped_(false)
        {

        }

        Thread::~Thread(void)
        {
            if (ev_dummy_ != NULL) {
                ::event_free(ev_dummy_);
            }

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

                if ((ev_dummy_ = event_new(ev_base_, -1, EV_TIMEOUT | EV_PERSIST, 
                    &Thread::OnDummyTimeout, this)) == NULL) {
                    break;
                }

                struct timeval tv = { 1, 0 };
                if (0 != event_add(ev_dummy_, &tv))
                    break;

                return ::pear::sys::Thread::Start();
            } while (false);

            if (ev_dummy_ != NULL) {
                ::event_free(ev_dummy_);
                ev_dummy_ = NULL;
            }

            if (ev_base_ != NULL) {
                ::event_base_free(ev_base_);
                ev_base_ = NULL;
            }

            return false;
        }

        void Thread::Stop(void)
        {
            if (!stopped_ && ev_base_ != NULL) {
                stopped_ = true;
                ::event_base_loopbreak(ev_base_);
            }
        }

        void Thread::Run(void)
        {
            assert(ev_base_ != NULL);
            ::event_base_dispatch(ev_base_);
        }

        void Thread::OnDummyTimeout(evutil_socket_t sock, short event, void *)
        {

        }
    }
}