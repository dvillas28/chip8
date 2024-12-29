#include "logic.h"

void op_00E0(ChipContext *ctx)
{
    /* clear screen */
    for (int i = 0; i < CTX_WIDTH * CTX_HEIGHT; i++)
    {
        ctx->display[i] = 0x00;
    }
}

void op_00EE(ChipContext *ctx)
{
    /* return */
    ctx->SP--;                     // decrement from stack
    ctx->PC = ctx->stack[ctx->SP]; // pop value
}

void op_1NNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3)
{
    /* jump */
    ctx->PC = (n1 << 8) | (n2 << 4) | n3;
}

void op_2NNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3)
{
    /* call subroutine */
    ctx->stack[ctx->SP] = ctx->PC; // push current instruction to stack
    ctx->SP++;                     // increment stack pointer

    ctx->PC = (n1 << 8) | (n2 << 4) | n3;
}

void op_3XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2)
{
    /* skip if equal */
    if (ctx->V[x] == ((n1 << 4) | n2))
    {
        ctx->PC += 2;
    }
}

void op_4XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2)
{
    /* skip if not equal */
    if (ctx->V[x] != ((n1 << 4) | n2))
    {
        ctx->PC += 2;
    }
}

void op_5XY0(ChipContext *ctx, u8 x, u8 y)
{
    /* skip if Vx == Vy */
    if (ctx->V[x] == ctx->V[y])
    {
        ctx->PC += 2;
    }
}

void op_9XY0(ChipContext *ctx, u8 x, u8 y)
{
    /* skip if Vx != Vy */
    if (ctx->V[x] != ctx->V[y])
    {
        ctx->PC += 2;
    }
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

void op_8XY0(ChipContext *ctx, u8 x, u8 y)
{
    /* Vx <- Vy */
    ctx->V[x] = ctx->V[y];
}

void op_8XY1(ChipContext *ctx, u8 x, u8 y)
{
    /* Vx <- Vx OR Vy */
    ctx->V[x] |= ctx->V[y];
}

void op_8XY2(ChipContext *ctx, u8 x, u8 y)
{
    /* Vx <- Vx AND Vy */
    ctx->V[x] &= ctx->V[y];
}

void op_8XY3(ChipContext *ctx, u8 x, u8 y)
{
    /* Vx <- Vx XOR Vy */
    ctx->V[x] ^= ctx->V[y];
}

void op_8XY4(ChipContext *ctx, u8 x, u8 y)
{
    /* Vx <- Vx + Vy, this will affect the carry flag */
    u16 sum = ctx->V[x] + ctx->V[y];
    // if result is greater than 8 bits
    if (sum > 255U)
    {
        ctx->V[0xF] = 1;
    }
    else
    {
        ctx->V[0xF] = 0;
    }

    // mantain only the 8 LSB
    ctx->V[x] = sum & 0xFFU;
}

void op_8XY5(ChipContext *ctx, u8 x, u8 y)
{
    /* Vx <- Vx - Vy, this will affect the carry flag */
    if (ctx->V[x] > ctx->V[y])
    {
        ctx->V[0xF] = 1;
    }
    else
    {
        ctx->V[0xF] = 0;
    }

    ctx->V[x] -= ctx->V[y];
}

void op_8XY7(ChipContext *ctx, u8 x, u8 y)
{
    /* Vx <- Vy - Vx, this will affect the carry flag */
    if (ctx->V[y] > ctx->V[x])
    {
        ctx->V[0xF] = 1;
    }
    else
    {
        ctx->V[0xF] = 0;
    }

    ctx->V[x] = ctx->V[y] - ctx->V[x];
}

void op_8XY6(ChipContext *ctx, u8 x, u8 y)
{
    /* Vx <- SHR Vx */
    // TODO: add via configuration mode this before Vx <- Vy

    // store the LSB in VF
    ctx->V[0xF] = ctx->V[x] & 0x1U;
    ctx->V[x] >>= 1;
}

void op_8XYE(ChipContext *ctx, u8 x, u8 y)
{
    /* Vx <- SHL Vx */
    // TODO: add via configuration mode this before Vx <- Vy

    // store the MSB in VF
    ctx->V[0xF] = (ctx->V[x] & 0x8U) >> 7u;
    ctx->V[x] <<= 1;
}

void op_ANNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3)
{
    /* sets the index register I to the value NNN */
    ctx->I = (n1 << 8) | (n2 << 4) | n3;
}

void op_BNNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3)
{
    /* jump with offset */
    ctx->PC = ctx->V[0] + ((n1 << 8) | (n2 << 4) | n3);
}

void op_CXNN(ChipContext *ctx, u8 x, u8 n1, u8 n2)
{
    /* Vx <- rand() & NN */
    u8 byte = (n1 << 4) | n2;

    ctx->V[x] = rand() & byte;
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

void op_EX9E(ChipContext *ctx, u8 x)
{
    /* skip if key[Vx] is pressed */

    u8 key = ctx->V[x];

    if (ctx->keypad[key] == 1)
    {
        ctx->PC += 2;
    }

    return;
}

void op_EXA1(ChipContext *ctx, u8 x)
{
    /* skip if key[Vx] is not pressed */

    u8 key = ctx->V[x];

    if (ctx->keypad[key] == 0)
    {
        ctx->PC += 2;
    }
}

void op_FX07(ChipContext *ctx, u8 x)
{
    /* Vx <- delay_reg */
    ctx->V[x] = ctx->delay_reg;
}

void op_FX15(ChipContext *ctx, u8 x)
{
    /* delay_reg <- Vx*/
    ctx->delay_reg = ctx->V[x];
}

void op_FX18(ChipContext *ctx, u8 x)
{
    /* sound_reg <- Vx*/
    ctx->sound_reg = ctx->V[x];
}

void op_FX1E(ChipContext *ctx, u8 x)
{
    /* add to index */
    ctx->I += ctx->V[x];
}

void op_FX0A(ChipContext *ctx, u8 x)
{
    /* get key */
    // decrement pc unless a key is pressed
    bool over = false;

    for (u8 key = 0x0; key <= 0xF; key++)
    {
        if (ctx->keypad[key] == 1)
        {
            ctx->V[x] = key;
            over = true;
        }
    }

    // if a keypad value was not detected
    if (!over)
    {
        ctx->PC -= 2;
    }
}

void op_FX29(ChipContext *ctx, u8 x)
{
    /* font character */
    ctx->I = FONTSET_START_ADDRESS + (5 * ctx->V[x]);
}

void op_FX33(ChipContext *ctx, u8 x)
{
    /* binary-coded decimal conversion */
    u8 number = ctx->V[x];

    // unit
    ctx->memory[ctx->I + 2] = number % 10;
    number /= 10;

    // tens
    ctx->memory[ctx->I + 1] = number % 10;
    number /= 10;

    // hundreds
    ctx->memory[ctx->I] = number % 10;
}

void op_FX55(ChipContext *ctx, u8 x)
{
    /* store memory */
    u8 Vx = ctx->V[x];

    for (u8 i = 0; i <= Vx; i++)
    {
        ctx->memory[ctx->I + i] = ctx->V[i];
    }
}

void op_FX65(ChipContext *ctx, u8 x)
{
    /* load memory */
    u8 Vx = ctx->V[x];

    for (u8 i = 0; i <= Vx; i++)
    {
        ctx->V[i] = ctx->memory[ctx->I + i];
    }
}

void chip_cycle(ChipContext *ctx)
{
    u16 opcode;

    // fetch
    opcode = (ctx->memory[ctx->PC] << 8u) | ctx->memory[ctx->PC + 1];
    ctx->PC += 2;

    decode(opcode, ctx);

    if (ctx->delay_reg > 0)
    {
        ctx->delay_reg--;
    }

    if (ctx->sound_reg > 0)
    {
        ctx->sound_reg--;
    }
}

void decode(u16 opcode, ChipContext *ctx)
{
    u8 first = (opcode >> 12) & 0xF;
    u8 second = (opcode >> 8) & 0xF;
    u8 third = (opcode >> 4) & 0xF;
    u8 fourth = opcode & 0xF;

    // printf("opcode: %x%x%x%x\n", first, second, third, fourth);

    switch (first)
    {
    case 0x0:
        switch ((third << 4) | fourth)
        {
        case 0xE0:
            op_00E0(ctx); // clear screen
            break;

        case 0xEE:
            op_00EE(ctx); // return
            break;

        default:
            break;
        }
        break;

    case 0x1:
        op_1NNN(ctx, second, third, fourth); // jump
        break;

    case 0x2:
        op_2NNN(ctx, second, third, fourth); // call subroutine
        break;

    case 0x3:
        op_3XNN(ctx, second, third, fourth); // skip if equal
        break;

    case 0x4:
        op_4XNN(ctx, second, third, fourth); // skip if not equal
        break;

    case 0x5:
        op_5XY0(ctx, second, third); // skip if Vx == Vy
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
            op_8XY0(ctx, second, third); // Vx <- Vy
            break;

        case 0x1:
            op_8XY1(ctx, second, third); // Vx <- Vx OR Vy
            break;

        case 0x2:
            op_8XY2(ctx, second, third); // Vx <- Vx AND Vy
            break;

        case 0x3:
            op_8XY3(ctx, second, third); // Vx <- Vx XOR Vy
            break;

        case 0x4:
            op_8XY4(ctx, second, third); // Vx + Vy, this will affect the carry flag
            break;

        case 0x5:
            op_8XY5(ctx, second, third); // Vx <- Vx - Vy, this will affect the carry flag
            break;

        case 0x6:
            op_8XY6(ctx, second, third); // Vx <- SHR Vx
            break;

        case 0x7:
            op_8XY7(ctx, second, third); // Vx <- Vy - Vx, this will affect the carry flag
            break;

        case 0xE:
            op_8XYE(ctx, second, third); // Vx <- SHL Vx
            break;

        default:
            break;
        }
        break;

    case 0x9:
        op_9XY0(ctx, second, third); // skip if Vx != Vy
        break;

    case 0xA:
        op_ANNN(ctx, second, third, fourth); // sets the index register I to the value nnn
        break;

    case 0xB:
        op_BNNN(ctx, second, third, fourth); // jump with offset
        break;

    case 0xC:
        op_CXNN(ctx, second, third, fourth); // Vx <- rand() & NN
        break;

    case 0xD:
        op_DXYN(ctx, second, third, fourth); // draw/display
        break;

    case 0xE:
        switch ((third << 4) | fourth)
        {
        case 0x9E:
            op_EX9E(ctx, second); // skip if key[Vx] is pressed
            break;

        case 0xA1:
            op_EXA1(ctx, second); // skip if key[Vx] is not pressed
            break;

        default:
            break;
        }
        break;

    case 0xF:
        switch ((third << 4) | fourth)
        {
        case 0x07:
            op_FX07(ctx, second); // Vx <- delay_reg
            break;

        case 0x0A:
            op_FX0A(ctx, second); // get key
            break;

        case 0x15:
            op_FX15(ctx, second); // delay_reg <- Vx
            break;

        case 0x18:
            op_FX18(ctx, second); // sound_reg <- Vx
            break;

        case 0x1E:
            op_FX1E(ctx, second); // add to index
            break;

        case 0x29:
            op_FX29(ctx, second); // font character
            break;

        case 0x33:
            op_FX33(ctx, second); // binary-coded decimal conversion
            break;

        case 0x55:
            op_FX55(ctx, second); // store memory
            break;

        case 0x65:
            op_FX65(ctx, second); // load memory
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}
