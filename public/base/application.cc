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

#include "application.h"
#include "exception.h"

#include <cassert>
#include <csignal>
#include <ctime>

namespace pear {
    namespace base {

        Application *Application::app_ = NULL;

        Application::Application(void)
            : arguemnts_()
            , options_()
            
            , up_time_(0)
            , log_core_()
            , log_(NULL)
        {
            assert(app_ == NULL);
            if (app_ != NULL) {
                exit(-1);
            }
            app_ = this;
        }

        Application::~Application(void)
        {

        }

        int Application::Run(int argc, char* argv[])
        {
            int rc = EXIT_OK;

            do 
            {
                try
                {
                    rc = Initialize(argc, argv);
                    if (rc != EXIT_OK) {
                        break;
                    }

                    ::time(&up_time_);

                    rc = Main(arguemnts_);
                }
                catch (Exception& /*e*/)
                {
                }
                catch (::std::exception& /*e*/)
                {
                }
                catch (...)
                {
                }

                Uninitialize();
            } while (false);

            return rc;
        }

        void Application::HookSignal(int signal)
        {
            ::signal(signal, &(Application::SignalHandler));
        }

        void Application::UnhookSignal(int signal)
        {
            ::signal(signal, NULL);
        }

        void Application::OnSignal(int signal)
        {

        }

        int Application::Initialize(int argc, char* argv[])
        {
            // process arguments
            for (int x = 1; x < argc; ++x) {
                arguemnts_.push_back(argv[x]);
            }

            OnDefineOptions(options_);
            options_.Process(arguemnts_);

            OptionSet::const_iterator it = options_.Begin();
            for (; it != options_.End(); ++it)
            {
                if (!it->is_valid())
                {
                    if (it->is_required()) {
                        return EXIT_USAGE;
                    }
                    continue;
                }

                int rc = OnOption(it->get_name(), it->get_value());
                if (rc != EXIT_OK) {
                    return rc;
                }
            }

            // initialize
            log_ = new ::pear::log::Log();

            // call user initialize
            return OnInitialize();
        }

        void Application::Uninitialize(void)
        {
            // call user uninitialize
            OnUninitialize();
        }

        void Application::SignalHandler(int signal)
        {
            app_->OnSignal(signal);
        }
    }
}