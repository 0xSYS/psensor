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



















#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <stdio.h>
#include <stdbool.h>



extern struct nk_xcb_context *xcb_ctx;
extern bool is_on_quit;



void render_main_window(struct nk_context * ctx)
{
    if(nk_begin(ctx, "nk - psensor", nk_rect(1, 1, xcb_ctx->width, xcb_ctx->height), NK_WINDOW_BORDER))
    {
        nk_layout_row_begin(ctx, NK_STATIC, 25, 5);
        nk_layout_row_push(ctx, 75);
        if(nk_menu_begin_label(ctx, "Psensor", NK_TEXT_LEFT, nk_vec2(150, 200)))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            
            if(nk_menu_item_label(ctx, "Preferences", NK_TEXT_LEFT))
                log_info("Stuff\n");
            
            if(nk_menu_item_label(ctx, "Fan Controller", NK_TEXT_LEFT))
                log_info("fan ctrl\n");
            
            if(nk_menu_item_label(ctx, "Sensor Settings", NK_TEXT_LEFT))
                log_info("sensor settings\n");
            
            if(nk_menu_item_label(ctx, "Quit", NK_TEXT_LEFT))
                is_on_quit = true;
            
            nk_menu_end(ctx);
        }
        
        if(nk_menu_begin_label(ctx, "Help", NK_TEXT_LEFT, nk_vec2(80, 200)))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            
            if(nk_menu_item_label(ctx, "About", NK_TEXT_LEFT))
                log_info("Stuff\n");
            
            nk_menu_end(ctx);
        }
        
        
        float id = 0;
        static int col_index = -1;
        static int line_index = -1;
        static nk_bool show_markers = nk_true;
        float step = (2*3.141592654f) / 32;

        int i;
        int index = -1;
        
        

        /* line chart */
        id = 0;
        index = -1;
        nk_layout_row_dynamic(ctx, 32, 1);
        nk_checkbox_label(ctx, "Show markers", &show_markers);
        ctx->style.chart.show_markers = show_markers;
      
        nk_chart_end(ctx);

        /* mixed colored chart */
        // Just a demo for now
        nk_layout_row_dynamic(ctx, 500, 1);
        if(nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
            nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,0,255), nk_rgb(0,0,150),32, -1.0f, 1.0f);
            nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,255,0), nk_rgb(0,150,0), 32, -1.0f, 1.0f);
            for (id = 0, i = 0; i < 32; ++i) {
                nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                nk_chart_push_slot(ctx, (float)cos(id), 1);
                nk_chart_push_slot(ctx, (float)sin(id), 2);
                id += step;
            }
        }
        nk_chart_end(ctx);
    }
    nk_window_set_bounds(ctx, "nk - psensor", nk_rect(0, 0, xcb_ctx->width, xcb_ctx->height));
    nk_end(ctx);
}


#endif
