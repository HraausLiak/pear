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

#ifndef PUBLIC_BASE_OPTION_H_
#define PUBLIC_BASE_OPTION_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <string>

namespace pear {
    namespace base {

        class Option
        {
        public:
            Option(const ::std::string& name, const ::std::string& short_name, const ::std::string& description);
            ~Option(void);

            Option& is_required(bool required) {
                required_ = required;
                return *this;
            }
            bool is_required(void) const { return required_; }
            Option& set_argument(const ::std::string& argument) {
                argument_ = argument;
                return *this;
            }

            const ::std::string& get_name(void) const { return name_; }
            const ::std::string& get_value(void) const { return value_; }

            bool is_valid(void) const { return valid_; }
            void is_valid(bool valid) { valid_ = valid; }

            bool MatchName(const std::string& name);
            bool MatchShortName(const std::string& short_name);
            bool HasArgment(void) const { return argument_.length() > 0; }

            void set_value(const std::string& value) { value_ = value; }

        private:
            ::std::string name_;
            ::std::string short_name_;
            ::std::string description_;
            ::std::string argument_;
            bool required_;

            ::std::string value_;
            bool valid_;
        };

    }
}

#endif // PUBLIC_BASE_OPTION_H_
