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

#ifndef REALMD_APPLICATION_H_
#define REALMD_APPLICATION_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include "public/base/application.h"

class RealmdApplication : public pear::base::Application
{
public:
    RealmdApplication(void);
    virtual ~RealmdApplication(void);

protected:
    virtual void OnDefineOptions(pear::base::OptionSet& options);
    virtual int OnOption(const ::std::string& name, const ::std::string& arg);
    virtual int OnInitialize(void);
    virtual void OnUninitialize(void);
    virtual int Main(::std::vector<::std::string>& unknownArgs);
};

#endif // REALMD_APPLICATION_H_
