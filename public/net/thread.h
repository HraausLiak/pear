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

#ifndef PUBLIC_NET_THREAD_H_
#define PUBLIC_NET_THREAD_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <event2/event.h>
#include "public/sys/thread.h"

namespace pear {
    namespace net {

        class Thread : public ::pear::sys::Thread
        {
        public:
            Thread(void);
            virtual ~Thread(void);

            bool Start(void);
            virtual void Stop(void);
            virtual void Run(void);

            inline struct event_base *AcquirEventBase(void) {
                ++busy_load_;
                return ev_base_;
            }
            inline  void ReleaseEventBase(void) {
                --busy_load_;
            }

            inline int get_busy_load(void) {
                return busy_load_;
            }

        private:
            static void OnDummyTimeout(evutil_socket_t sock, short event, void *);

            struct event_base *ev_base_;
            struct event *ev_dummy_;
            volatile int busy_load_;
            bool stopped_;
        };

    }
}

#endif // PUBLIC_NET_THREAD_H_
