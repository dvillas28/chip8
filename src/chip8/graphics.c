#include "graphics.h"
#include "utils.h"
#include "input.h"

bool init_graphics(Window *win)
{
    int SCREEN_WIDTH = PIXEL_SIZE * CTX_WIDTH;
    int SCREEN_HEIGHT = PIXEL_SIZE * CTX_HEIGHT;

    // init flag
    bool success = true;

    // Initialization of SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    else
    {
        // create Window
        win->gWindow = SDL_CreateWindow("CHIP8-EMULATOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (win->gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }

        else
        {
            // get window surface
            win->gScreenSurface = SDL_GetWindowSurface(win->gWindow);
        }
    }

    return success;
}

void close_graphics(Window *win)
{

    // destroy window
    SDL_DestroyWindow(win->gWindow);
    win->gWindow = NULL;

    // quit SDL subsystem
    SDL_Quit();
}

void draw_display(Pixel *pixel, ChipContext *ctx, Window *win)
{
    u32 color;

    for (int i = 0; i < CTX_WIDTH; i++)
    {
        for (int j = 0; j < CTX_HEIGHT; j++)
        {
            pixel->rect.x = i * PIXEL_SIZE;
            pixel->rect.y = j * PIXEL_SIZE;
            if (ctx->display[j * 64 + i])
            {
                color = SDL_MapRGB(win->gScreenSurface->format, pixel->r, pixel->g, pixel->b);
            }
            else
            {
                color = SDL_MapRGB(win->gScreenSurface->format, 0x00, 0x00, 0x00);
            }

            // draw here
            SDL_FillRect(win->gScreenSurface, &pixel->rect, color);
        }
    }

    // update the surface
    SDL_UpdateWindowSurface(win->gWindow);
}
