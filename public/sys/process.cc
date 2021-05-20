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

#include "process.h"

#ifdef __WINDOWS__
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#endif

namespace pear {
    namespace sys {

        static char s_app_dir[MAX_PATH];
        static char s_app_name[MAX_PATH];


        const char *GetCurrentDirectory(void)
        {
#ifdef __WINDOWS__
            ::GetCurrentDirectoryA(MAX_PATH, s_app_dir);
#endif
            return s_app_dir;
        }

        const char *GetModuleName(void)
        {
#ifdef __WINDOWS__
            ::GetModuleFileNameA(NULL, s_app_name, MAX_PATH);
#endif
            return s_app_name;
        }
    }
}