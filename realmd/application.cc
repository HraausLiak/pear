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

#include <stdio.h>

RealmdApplication::RealmdApplication(void)
{

}

RealmdApplication::~RealmdApplication(void)
{

}

void RealmdApplication::OnDefineOptions(pear::base::OptionSet& options)
{
    options.AddOption(pear::base::Option("help", "h", "show usage").is_required(false));
}

int RealmdApplication::OnOption(const ::std::string& name, const ::std::string& arg)
{
    printf("RealmdApplication::OnOption: %s,%s\n", name.c_str(), arg.c_str());

    if (name == "help") {
        return EXIT_USAGE;
    }
    return EXIT_OK;
}

int RealmdApplication::OnInitialize(void)
{
    return EXIT_OK;
}

void RealmdApplication::OnUninitialize(void)
{

}

int RealmdApplication::Main(::std::vector<::std::string>& unknownArgs)
{
    return EXIT_OK;
}