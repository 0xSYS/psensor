#include <stdio.h>

#include "ui_main.h"
#include "main_window.h"
#include "nk_defs.c"




#define NK_XCB_CAIRO_IMPLEMENTATION
#include <nuklear_xcb.h>





struct nk_xcb_context *xcb_ctx;
struct nk_cairo_context *cairo_ctx;
struct nk_user_font *font;
struct nk_context* ctx;



void ui_init()
{
    struct nk_color background_color = nk_rgb(0, 0, 0); // Background color of the x11 window

    xcb_ctx = nk_xcb_init("Psensor", 20, 20, 1500, 950); //Create a rendering window with cairo via xcb

    // Create rendering surface using a custom font (DEFAULT_UI_FONT)
    cairo_ctx = nk_cairo_init(&background, DEFAULT_UI_FONT, 18, nk_xcb_create_cairo_surface(xcb_ctx));
    font = nk_cairo_default_font(cairo_ctx); // Set the loaded font
    ctx = malloc(sizeof(struct nk_context));
    nk_init_default(ctx, font); // Initialize the nuklear context with the font
    
    // Set default theme
    set_style(ctx, parsed_config.color_theme);
}


void ui_main()
{
    printf("Ui main\n");
}
