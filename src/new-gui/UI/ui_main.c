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














#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <log_c/log.h>


#include "ui_main.h"




#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#include <nuklear.h>

#define NK_XCB_CAIRO_IMPLEMENTATION
#include <nuklear_xcb.h>



#include "style.h"
#include "main_window.h"






struct nk_xcb_context *xcb_ctx;
struct nk_cairo_context *cairo_ctx;
struct nk_user_font *font;
struct nk_context* ctx;

bool is_on_quit;



void ui_init()
{
    struct nk_color background_color = nk_rgb(0, 0, 0); // Background color of the x11 window

    xcb_ctx = nk_xcb_init("Psensor", 20, 20, 1500, 950); //Create a rendering window with cairo via xcb

    // Create rendering surface using a custom font (DEFAULT_UI_FONT)
    cairo_ctx = nk_cairo_init(&background_color, DEFAULT_UI_FONT, 18, nk_xcb_create_cairo_surface(xcb_ctx));
    font = nk_cairo_default_font(cairo_ctx); // Set the loaded font
    ctx = malloc(sizeof(struct nk_context));
    nk_init_default(ctx, font); // Initialize the nuklear context with the font
    
    // Set default theme
    set_style(ctx, 5);
    
    log_info("UI Init");
}


void ui_main()
{
    log_info("UI main\n");


    int events;
    
    // This is required for switching in between multiple color themes (defined in style.c)
    static struct nk_color color_table[NK_COLOR_COUNT];
    memcpy(color_table, nk_default_color_style, sizeof(color_table));
    
    
    ui_init();


    while(1)
    {
        events = nk_xcb_handle_event(xcb_ctx, ctx);
        if(events & NK_XCB_EVENT_STOP)
        {
            break;
        }
        if(events & NK_XCB_EVENT_PAINT)
        {
            nk_cairo_damage(cairo_ctx);
        }
        if(events & NK_XCB_EVENT_RESIZED)
        {
            nk_xcb_resize_cairo_surface(xcb_ctx, nk_cairo_surface(cairo_ctx));
        }

        render_main_window(ctx);
       
        if(is_on_quit)
        {
            log_info("Exit event triggered");
            break;
        }

        nk_cairo_render(cairo_ctx, ctx);
        nk_xcb_render(xcb_ctx);
        nk_clear(ctx);
    }


    nk_free(ctx);
    free(ctx);
    nk_cairo_free(cairo_ctx);
    nk_xcb_free(xcb_ctx);
}
