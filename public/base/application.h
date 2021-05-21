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

#ifndef PUBLIC_BASE_APPLICATION_H_
#define PUBLIC_BASE_APPLICATION_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <string>
#include <vector>

#include "public/base/option.h"
#include "public/base/option_set.h"
#include "public/log/log_core.h"
#include "public/log/log.h"

namespace pear {
    namespace base {

        class Application
        {
        public:
            enum ExitCode
            {
                EXIT_OK = 0,
                EXIT_USAGE = 64,
                EXIT_CONFIG = 65,
                EXIT_SOFTWARE = 70,
            };

            Application(void);
            virtual ~Application(void);

            int Run(int argc, char* argv[]);

            void HookSignal(int signal);
            void UnhookSignal(int signal);

            static Application *instance(void) {
                return app_;
            }

            time_t uptime(void) const {
                return up_time_;
            }

            ::pear::log::Log *get_log() {
                return log_;
            }

        protected:
            virtual void OnDefineOptions(OptionSet& options) = 0;
            virtual int OnOption(const ::std::string& name, const ::std::string& arg) = 0;
            virtual void OnSignal(int signal);

            virtual int OnInitialize(void) = 0;
            virtual void OnUninitialize(void) = 0;

            virtual int Main(::std::vector<::std::string>& unknownArgs) = 0;

        private:
            int Initialize(int argc, char* argv[]);
            void Uninitialize(void);

            static void SignalHandler(int signal);


            ::std::vector<::std::string> arguemnts_;
            OptionSet options_;

            // application global data
            time_t up_time_;
            ::pear::log::LogCore log_core_;
            ::pear::log::Log *log_;

            static Application *app_;
        };

    }
}

#endif // PUBLIC_BASE_APPLICATION_H_
