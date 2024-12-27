#include "chip8.h"

int start_chip8(ChipContext *ctx)
{

    // start SDL and create window
    Window win;
    win.gScreenSurface = NULL;
    win.gWindow = NULL;

    if (!init_graphics(&win))
    {
        printf("Failed to initialize!\n");
    }
    else
    {

        // main loop flag
        bool quit = false;

        // event handler
        SDL_Event e;

        // rect to draw a pixel
        Pixel pixel;
        pixel.rect.h = PIXEL_SIZE;
        pixel.rect.w = PIXEL_SIZE;

        // color options
        pixel.r = 0x00;
        pixel.g = 0x89;
        pixel.b = 0x59;

        // while app is running - Main Loop
        while (!quit)
        {
            // process the keystrokes and use them if needed
            quit = process_input(quit, e, ctx);
            // print_keys(ctx);

            // check for the chip tick here?

            draw_display(&pixel, ctx, &win);
        }
        print_mem(ctx);

        // free resources and close SDL
        close_graphics(&win);
    }

    return 0;
}