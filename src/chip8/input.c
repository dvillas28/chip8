#include "input.h"

bool process_input(bool quit, SDL_Event e, ChipContext *ctx)
{

    // handle event on queue
    while (SDL_PollEvent(&e) != 0)
    {
        // user requests quit

        if (e.type == SDL_QUIT)
        {
            print_char('\n');
            quit = true;
        }
        // user presses a key
        else if (e.type == SDL_KEYDOWN)
        {
            // select what to do based on key press
            switch (e.key.keysym.sym)
            {

            case SDLK_1:
                ctx->keypad[0x0] = 1;
                ctx->display[0] = !ctx->display[0];
                break;

            case SDLK_2:
                ctx->keypad[0x1] = 1;
                ctx->display[1] = !ctx->display[1];
                break;

            case SDLK_3:
                ctx->keypad[0x2] = 1;
                ctx->display[2] = !ctx->display[2];
                break;

            case SDLK_4:
                ctx->keypad[0x3] = 1;
                break;

            case SDLK_q:
                ctx->keypad[0x4] = 1;
                break;

            case SDLK_w:
                ctx->keypad[0x5] = 1;
                break;

            case SDLK_e:
                ctx->keypad[0x6] = 1;
                break;

            case SDLK_r:
                ctx->keypad[0x7] = 1;
                break;

            case SDLK_a:
                ctx->keypad[0x8] = 1;
                break;

            case SDLK_s:
                ctx->keypad[0x9] = 1;
                break;

            case SDLK_d:
                ctx->keypad[0xA] = 1;
                break;

            case SDLK_f:
                ctx->keypad[0xB] = 1;
                break;

            case SDLK_z:
                ctx->keypad[0xC] = 1;
                break;

            case SDLK_x:
                ctx->keypad[0xD] = 1;
                break;

            case SDLK_c:
                ctx->keypad[0xE] = 1;
                break;

            case SDLK_v:
                ctx->keypad[0xF] = 1;
                break;

            default:
                break;
            }
        }

        else if (e.type == SDL_KEYUP)
        {
            switch (e.key.keysym.sym)
            {

            case SDLK_1:
                ctx->keypad[0x0] = 0;
                break;

            case SDLK_2:
                ctx->keypad[0x1] = 0;
                break;

            case SDLK_3:
                ctx->keypad[0x2] = 0;
                break;

            case SDLK_4:
                ctx->keypad[0x3] = 0;
                break;

            case SDLK_q:
                ctx->keypad[0x4] = 0;
                break;

            case SDLK_w:
                ctx->keypad[0x5] = 0;
                break;

            case SDLK_e:
                ctx->keypad[0x6] = 0;
                break;

            case SDLK_r:
                ctx->keypad[0x7] = 0;
                break;

            case SDLK_a:
                ctx->keypad[0x8] = 0;
                break;

            case SDLK_s:
                ctx->keypad[0x9] = 0;
                break;

            case SDLK_d:
                ctx->keypad[0xA] = 0;
                break;

            case SDLK_f:
                ctx->keypad[0xB] = 0;
                break;

            case SDLK_z:
                ctx->keypad[0xC] = 0;
                break;

            case SDLK_x:
                ctx->keypad[0xD] = 0;
                break;

            case SDLK_c:
                ctx->keypad[0xE] = 0;
                break;

            case SDLK_v:
                ctx->keypad[0xF] = 0;
                break;

            default:
                break;
            }
        }
    }

    return quit;
}