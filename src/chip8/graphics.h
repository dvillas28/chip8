#pragma once
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#include "context.h"

#define PIXEL_SIZE 20

// Contains pointers to the SDL_Window we will rendering to, and the SDL_Surface contained by it.
typedef struct Window
{
    // the window we will be rendering to
    SDL_Window *gWindow;

    // the surface contained by the window
    SDL_Surface *gScreenSurface;
} Window;

// Contains the SDL_Rect object that will represent a pixel on a SDL_Surface.
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