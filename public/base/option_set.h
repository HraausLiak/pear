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

#ifndef PUBLIC_BASE_OPTION_SET_H_
#define PUBLIC_BASE_OPTION_SET_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <vector>
#include <string>
#include "public/base/option.h"

namespace pear {
    namespace base {

        class OptionSet
        {
        public:
            typedef ::std::vector<Option>::iterator iterator;
            typedef ::std::vector<Option>::const_iterator const_iterator;

            OptionSet(void);
            ~OptionSet(void);

            iterator Begin(void) {
                return options_.begin();
            }
            iterator End(void) {
                return options_.end();
            }
            const_iterator Begin(void) const {
                return options_.begin();
            }
            const_iterator End(void) const {
                return options_.end();
            }

            void AddOption(Option& option);
            void Process(::std::vector<::std::string>& args);

        private:
            ::std::vector<Option> options_;
        };

    }
}

#endif // PUBLIC_BASE_OPTION_SET_H_
