#pragma once
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#include "context.h"

#define PIXEL_SIZE 20

typedef struct Window
{
    // the window we will be rendering to
    SDL_Window *gWindow;

    // the surface contained by the window
    SDL_Surface *gScreenSurface;
} Window;

typedef struct Pixel
{
    SDL_Rect rect;

    // color options for the window
    int r;
    int g;
    int b;
} Pixel;

bool init_graphics(Window *win);
void close_graphics(Window *win);
void draw_display(Pixel *pixel, ChipContext *ctx, Window *win);