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
 
 
#include <libkmod.h>
#include <stdlib.h>
#include "include/psensor/pmod.h"
#include <log_c/log.h>
 
 
struct kmod_ctx *kmod_ctx;
struct kmod_module *modules[MAX_MODULES];
 



int create_mod_obj(const char * mod_name, struct kmod_ctx *ctx,  struct kmod_module **mod)
{
    if(kmod_module_new_from_name(ctx, mod_name, mod) < 0)
    {
        log_error("Failed to create module object for %s", mod_name);
        kmod_unref(ctx);
        return -1;
    }
    return 0;
}

void pmod_load_modules()
{
    // Load necessary modules here
    for(int i = 0; module_table[i].name != NULL; i++)
    {
        if(create_mod_obj(module_table[i].name, kmod_ctx, &modules[i]) == 0)
        {
            if(kmod_module_insert_module(modules[i], 0,  module_table[i].opts) == 0)
                log_info("Loaded module: %s", module_table[i].name);
            else
                log_error("Failed to load module: %s", module_table[i].name);
        }
    }
    
    // Free stuff
    for(int i = 0; module_table[i].name != NULL; i++)
        kmod_module_unref(modules[i]);
    
}

void pmod_init()
{
    kmod_ctx = kmod_new(NULL, NULL);
    if(!kmod_ctx)
    {
        log_error("Failed to create kmod context");
        exit(1);
    }
    else
    {
        printf("mod Init\n");
    }
}

bool pmod_check_loaded(const char * mod_name)
{
    struct kmod_module *m;
    enum kmod_module_initstate state;
    create_mod_obj(mod_name, kmod_ctx, &m);
    
    state = kmod_module_get_initstate(m);
    if(state == KMOD_MODULE_LIVE)
    {
        log_info("Module %s is loaded", mod_name);
        kmod_module_unref(m);
        return true;
    }
    else if(state == KMOD_MODULE_BUILTIN)
    {
        log_info("Module %s is built into the kernel", mod_name);
        kmod_module_unref(m);
        return true;
    }
    else
    {
        log_error("Module %s is not loaded (state: %d)", mod_name, state);
        kmod_module_unref(m);
        return false;
    }
}