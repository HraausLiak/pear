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

#ifndef PUBLIC_NET_SOCKET_H_
#define PUBLIC_NET_SOCKET_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstddef>
#include <cstdint>
#include <string>

#include <event2/util.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

namespace pear {
    namespace net {

        class Socket
        {
        public:
            Socket(void);

            virtual ~Socket(void);

            bool Open(struct event_base *ev_base, ::evutil_socket_t fd, struct sockaddr *sa = NULL, int socklen = 0);
            void Close(void);

            inline const ::std::string& get_host(void) const {
                return host_;
            }
            inline int get_port(void) const {
                return port_;
            }
            inline const ::std::string& get_error_string(void) const {
                return error_string_;
            }
            inline int get_errno(void) const {
                return errno_;
            }

            void Send(const ::std::uint8_t *data, ::std::size_t size);
            inline ::std::size_t Receive(::std::uint8_t *data, ::std::size_t size) {
                return ::evbuffer_remove(bufferevent_get_input(bev_), data, size);
            }
            inline ::std::size_t ReceivePeek(::std::uint8_t *data, ::std::size_t size) {
                return ::evbuffer_copyout(bufferevent_get_input(bev_), data, size);
            }
            inline ::std::size_t ReceiveSkip(::std::size_t size) {
                if (evbuffer_drain(bufferevent_get_input(bev_), size) == 0) {
                    return size;
                }
                return 0;
            }

        protected:
            virtual void OnOpen(void) = 0;
            virtual void OnRead(void) = 0;
            virtual void OnWrite(void) = 0;
            virtual void OnEof(void) {}
            virtual void OnClose(void) = 0;
            virtual void OnError(void) = 0;
            virtual void OnTimeout(void) {}

        private:
            void EofOrError(void);

            static void ReadCallback(struct bufferevent *, void *);
            static void WriteCallback(struct bufferevent *, void *);
            static void EventCallback(struct bufferevent *, short, void *);

            evutil_socket_t fd_;
            struct bufferevent *bev_;


            ::std::string host_;
            int port_;

            ::std::string error_string_;
            int errno_;

            volatile bool fatal_error_;
        };

    }
}

#endif // PUBLIC_NET_SOCKET_H_
