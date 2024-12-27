#pragma once
#include <SDL.h>
#include <stdbool.h>
#include "context.h"
#include "utils.h"

bool process_input(bool quit, SDL_Event e, ChipContext *ctx);