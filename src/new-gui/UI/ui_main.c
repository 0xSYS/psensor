#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "nk_defs.c"
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
}


void ui_main()
{
    printf("Ui main\n");


    int events;
    
    // This is required for switching in between multiple color themes (defined in style.c)
    static struct nk_color color_table[NK_COLOR_COUNT];
    memcpy(color_table, nk_default_color_style, sizeof(color_table));
    
    
    
    ui_init();

    //int i = 0;
    while(1)
    {
        events = nk_xcb_handle_event(xcb_ctx, ctx);
        if(events & NK_XCB_EVENT_STOP)
        {
            break;
        }
        if(events & NK_XCB_EVENT_PAINT)
        {
            // Not sure but I think this is resposnable for clearing the previous frame...
            nk_cairo_damage(cairo_ctx);
        }
        if(events & NK_XCB_EVENT_RESIZED)
        {
            //i++;
            //printf("Window resized: %d\n", i);
            // Always keep the rendering surface sized as the window
            //printf("Window size: %d x %d\n", xcb_ctx->width, xcb_ctx->height);
            nk_xcb_resize_cairo_surface(xcb_ctx, nk_cairo_surface(cairo_ctx));
        }


 #ifdef UI_DEVEL
        if(nk_begin(ctx, "[Dev] - Demo", nk_rect(1, 1, xcb_ctx->width, xcb_ctx->height), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            //
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                fprintf(stdout, "button pressed\n");
            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
        }
        nk_window_set_bounds(ctx, "[Dev] - Demo", 
            nk_rect(0, 0, xcb_ctx->width, xcb_ctx->height));
        nk_end(ctx);
        if(nk_window_is_hidden(ctx, "[Dev] - Demo"))
        {
            break;
        }
        
        if(nk_begin(ctx, "[Dev] - unicodes", nk_rect(230, 230, 230, 95), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            nk_layout_row_static(ctx, 30, 170, 1);
            if (nk_button_label(ctx, "ă Ă î Î â Â ș Ș ț Ț"))
                fprintf(stdout, "unicode button pressed\n");
        }
        nk_end(ctx);
        if(nk_window_is_hidden(ctx, "[Dev] - unicodes"))
        {
            break;
        }
#endif

       render_main_window(ctx);


       nk_cairo_render(cairo_ctx, ctx);
       nk_xcb_render(xcb_ctx);
       nk_clear(ctx);
        
    }


    nk_free(ctx);
    free(ctx);
    nk_cairo_free(cairo_ctx);
    nk_xcb_free(xcb_ctx);
}
