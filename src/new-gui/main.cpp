/*
 * Copyright (C) 2025 xsys061@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */


#include <iostream>












//#include <stdio.h>


//#define DEV_TEST



#ifndef DEV_TEST
    #include "ui_entry.hpp"
    #include "utils.hpp"
#else
    #include "tests.h"
#endif





int main(int argc, char * argv[])
{
#ifndef DEV_TEST
    Utils::setup_dirs();
    ui_main();
#else
    test_stuff();
#endif

    return 0;
}
