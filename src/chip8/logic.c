#include "logic.h"

void op_00E0(ChipContext *ctx)
{
    /* clear screen */
    for (int i = 0; i < CTX_WIDTH * CTX_HEIGHT; i++)
    {
        ctx->display[i] = 0x00;
    }
}

void op_1NNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3)
{
    /* jump */
    ctx->PC = (n1 << 8) | (n2 << 4) | n3;
}

void op_6XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2)
{
    /* set register vx to the value nn */
    ctx->V[x] = (n1 << 4) | n2;
}

void op_7XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2)
{
    /* add value nn to vx */
    // the flag register vF is NOT affected
    ctx->V[x] += (n1 << 4) | n2;
}

void op_ANNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3)
{
    /* sets the index register I to the value NNN */
    ctx->I = (n1 << 8) | (n2 << 4) | n3;
}

void op_DXYN(ChipContext *ctx, u8 x, u8 y, u8 n)
{
    /*
    draw an N pixels tall sprite from the memory location that the I index register is holding to
    the screen, at the horizontal X coordinate in VX and the Y coordinate in VY
    */

    u8 xPos = ctx->V[x] % 64;
    u8 yPos = ctx->V[y] % 32;

    ctx->V[0xF] = 0;

    u8 sprite_byte;
    u8 sprite_pixel;
    b8 *screen_pixel;

    for (u8 row = 0; row < n; ++row)
    {
        sprite_byte = ctx->memory[ctx->I + row];

        for (u8 col = 0; col < 8; ++col)
        {
            sprite_pixel = sprite_byte & (0x80u >> col);
            screen_pixel = &(ctx->display[(yPos + row) * CTX_WIDTH + (xPos + col)]);

            if (sprite_pixel)
            {

                // screen pixel also on - collision
                if (*screen_pixel == 0xFFFFFFFF)
                {
                    ctx->V[0xF] = 1;
                }

                *screen_pixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void chip_cycle(ChipContext *ctx)
{
    u16 opcode;

    opcode = (ctx->memory[ctx->PC] << 8u) | ctx->memory[ctx->PC + 1];
    ctx->PC += 2;

    decode(opcode, ctx);
}

void decode(u16 opcode, ChipContext *ctx)
{
    u8 first = (opcode >> 12) & 0xF;
    u8 second = (opcode >> 8) & 0xF;
    u8 third = (opcode >> 4) & 0xF;
    u8 fourth = opcode & 0xF;

    switch (first)
    {
    case 0x0:
        switch ((third << 4) | fourth)
        {
        case 0xE0:
            op_00E0(ctx); // clear screen
            break;

        case 0xEE:
            /* 00EE */
            break;

        default:
            break;
        }
        break;

    case 0x1:
        op_1NNN(ctx, second, third, fourth); // jump
        break;

    case 0x2:
        /* 2NNN */
        break;

    case 0x3:
        /* 3XNN */
        break;

    case 0x4:
        /* 4XNN */
        break;

    case 0x5:
        /* 5XY0 */
        break;

    case 0x6:
        op_6XNN(ctx, second, third, fourth); // set register vx to nn
        break;

    case 0x7:
        op_7XNN(ctx, second, third, fourth); // add value nn to vx, the flag register vF is NOT affected
        break;

    case 0x8:
        switch (fourth)
        {
        case 0x0:
            /* 8XY0 */
            break;

        case 0x1:
            /* 8XY1 */
            break;

        case 0x2:
            /* 8XY2 */
            break;

        case 0x3:
            /* 8XY3 */
            break;

        case 0x4:
            /* 8XY4 */
            break;

        case 0x5:
            /* 8XY5 */
            break;

        case 0x6:
            /* 8XY6 */
            break;

        case 0x7:
            /* 8XY7 */
            break;

        case 0xE:
            /* 8XYE */
            break;

        default:
            break;
        }
        break;

    case 0x9:
        /* 9XY0 */
        break;

    case 0xA:
        op_ANNN(ctx, second, third, fourth); // sets the index register I to the value nnn
        break;

    case 0xB:
        /* BNNN */
        break;

    case 0xC:
        /* CXNN */
        break;

    case 0xD:
        op_DXYN(ctx, second, third, fourth); // draw/display
        break;

    case 0xE:
        switch ((third << 4) | fourth)
        {
        case 0x9E:
            /* EX9E */
            break;

        case 0xA1:
            /*EXA1*/
            break;

        default:
            break;
        }
        break;

    case 0xF:
        switch ((third << 4) | fourth)
        {
        case 0x07:
            /* FX07 */
            break;

        case 0x0A:
            /* FX0A */
            break;

        case 0x15:
            /* FX15 */
            break;

        case 0x18:
            /* FX18 */
            break;

        case 0x1E:
            /* FX1E */
            break;

        case 0x29:
            /* FX29 */
            break;

        case 0x33:
            /* FX33 */
            break;

        case 0x55:
            /* FX55 */
            break;

        case 0x65:
            /* FX65 */
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}
