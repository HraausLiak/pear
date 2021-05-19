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

#include "option_set.h"

namespace pear {
    namespace base {


        OptionSet::OptionSet(void)
            : options_()
        {

        }

        OptionSet::~OptionSet(void)
        {

        }

        void OptionSet::AddOption(Option& option)
        {
            ::std::vector<Option>::const_iterator it = options_.begin();
            for (; it != options_.end(); ++it)
            {
                if (it->get_name() == option.get_name())
                    return;
            }
            options_.push_back(option);
        }

        void OptionSet::Process(::std::vector<::std::string>& args)
        {
            ::std::vector<std::string>::const_iterator arg_iter = args.begin();
            for (; arg_iter != args.end();)
            {
                bool unknown_arg = true;
                ::std::vector<Option>::iterator option_iter = options_.begin();
                for (; (option_iter != options_.end() && arg_iter != args.end()); ++option_iter)
                {
                    if (option_iter->MatchName(*arg_iter) || option_iter->MatchShortName(*arg_iter))
                    {
                        arg_iter = args.erase(arg_iter);
                        if (option_iter->HasArgment() && arg_iter != args.end())
                        {
                            option_iter->set_value(*arg_iter);
                            option_iter->is_valid(true);
                            arg_iter = args.erase(arg_iter);
                        }
                        else if (!option_iter->HasArgment()) {
                            option_iter->is_valid(true);
                        }
                        unknown_arg = false;

                        break;
                    }
                }

                if (unknown_arg) {
                    // skip unknown arg
                    ++arg_iter;
                }
            }
        }

    }
}