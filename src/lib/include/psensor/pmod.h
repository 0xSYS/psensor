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
 
 
#ifndef PSENSOR_PMOD_H
#define PSENSOR_PMOD_H

#include <stdio.h>
#include <stdbool.h>
#include <libkmod.h>


#define MAX_MODULES 32

static const char *required_modules[] =
{
    "coretemp",
    "nct6775",
    "k10temp",
    "it87",
    NULL
};

void pmod_init();
void pmod_load_modules();
bool pmod_check_loaded(const char * mod_name);

#endif 