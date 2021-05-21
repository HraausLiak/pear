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

#include "option.h"

namespace pear {
    namespace base {


        Option::Option(const std::string& name, const std::string& short_name, const std::string& description)
            : name_(name)
            , short_name_(short_name)
            , description_(description)
            , argument_("")
            , required_(false)
            , value_("")
            , valid_(false)
        {

        }

        Option::~Option(void)
        {

        }

        bool Option::MatchName(const std::string& name)
        {
            if (name[0] != '-' && name[1] != '-') {
                return false;
            }
            return (name_.compare(&name[2]) == 0);
        }

        bool Option::MatchShortName(const std::string& short_name)
        {
            if (short_name[0] != '-') {
                return false;
            }
            return (short_name_.compare(&short_name[1]) == 0);
        }

    }
}