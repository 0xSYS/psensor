#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <stdio.h>

extern struct nk_xcb_context *xcb_ctx;



void render_main_window(struct nk_context * ctx)
{
    //printf("Main window\n");
    if(nk_begin(ctx, "nk - psensor", nk_rect(1, 1, xcb_ctx->width, xcb_ctx->height), NK_WINDOW_BORDER))
    {
        nk_layout_row_begin(ctx, NK_STATIC, 25, 5);
        nk_layout_row_push(ctx, 75);
        if(nk_menu_begin_label(ctx, "Psensor", NK_TEXT_LEFT, nk_vec2(150, 200)))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            
            if(nk_menu_item_label(ctx, "Preferences", NK_TEXT_LEFT))
                printf("Stuff\n");
            
            if(nk_menu_item_label(ctx, "Fan Controller", NK_TEXT_LEFT))
                printf("fan ctrl\n");
            
            if(nk_menu_item_label(ctx, "Sensor Settings", NK_TEXT_LEFT))
                printf("sensor settings\n");
            
            if(nk_menu_item_label(ctx, "Quit", NK_TEXT_LEFT))
                printf("exit\n");
            
            nk_menu_end(ctx);
        }
        
        if(nk_menu_begin_label(ctx, "Help", NK_TEXT_LEFT, nk_vec2(80, 200)))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            
            if(nk_menu_item_label(ctx, "About", NK_TEXT_LEFT))
                printf("Stuff\n");
            
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
        nk_layout_row_dynamic(ctx, 15, 1);
        nk_checkbox_label(ctx, "Show markers", &show_markers);
        nk_layout_row_dynamic(ctx, 100, 1);
        ctx->style.chart.show_markers = show_markers;
        if (nk_chart_begin(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f)) {
            for (i = 0; i < 32; ++i) {
                nk_flags res = nk_chart_push(ctx, (float)cos(id));
                if (res & NK_CHART_HOVERING)
                    index = (int)i;
                if (res & NK_CHART_CLICKED)
                    line_index = (int)i;
                id += step;
            }
            nk_chart_end(ctx);
        }

        if (index != -1)
            nk_tooltipf(ctx, "Value: %.2f", (float)cos((float)index*step));
        if (line_index != -1) {
            nk_layout_row_dynamic(ctx, 20, 1);
            nk_labelf(ctx, NK_TEXT_LEFT, "Selected value: %.2f", (float)cos((float)index*step));
        }

        /* column chart */
        nk_layout_row_dynamic(ctx, 100, 1);
        if (nk_chart_begin(ctx, NK_CHART_COLUMN, 32, 0.0f, 1.0f)) {
            for (i = 0; i < 32; ++i) {
                nk_flags res = nk_chart_push(ctx, (float)fabs(sin(id)));
                if (res & NK_CHART_HOVERING)
                    index = (int)i;
                if (res & NK_CHART_CLICKED)
                    col_index = (int)i;
                id += step;
            }
            nk_chart_end(ctx);
        }
        if (index != -1)
            nk_tooltipf(ctx, "Value: %.2f", (float)fabs(sin(step * (float)index)));
        if (col_index != -1) {
            nk_layout_row_dynamic(ctx, 20, 1);
            nk_labelf(ctx, NK_TEXT_LEFT, "Selected value: %.2f", (float)fabs(sin(step * (float)col_index)));
        }

        /* mixed chart */
        nk_layout_row_dynamic(ctx, 100, 1);
        if (nk_chart_begin(ctx, NK_CHART_COLUMN, 32, 0.0f, 1.0f)) {
            nk_chart_add_slot(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f);
            nk_chart_add_slot(ctx, NK_CHART_LINES, 32, -1.0f, 1.0f);
            for (id = 0, i = 0; i < 32; ++i) {
                nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
                nk_chart_push_slot(ctx, (float)cos(id), 1);
                nk_chart_push_slot(ctx, (float)sin(id), 2);
                id += step;
            }
        }
        nk_chart_end(ctx);

        /* mixed colored chart */
        nk_layout_row_dynamic(ctx, 100, 1);
        if (nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255,0,0), nk_rgb(150,0,0), 32, 0.0f, 1.0f)) {
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
        //nk_tree_pop(ctx);
    
    }
    nk_window_set_bounds(ctx, "nk - psensor", nk_rect(0, 0, xcb_ctx->width, xcb_ctx->height));
    nk_end(ctx);
    
    //if(nk_window_is_hidden(ctx, "nk - psensor"))
    //{
    //    break;
    //}
}


#endif
