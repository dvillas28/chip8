#include "logic.h"

/* Clear screen: clear the display, turning all pixels off to 0. */
void op_00E0(ChipContext *ctx)
{
    for (int i = 0; i < CTX_WIDTH * CTX_HEIGHT; i++)
    {
        ctx->display[i] = 0x00;
    }
}

/* Return: Return from subroutine. */
void op_00EE(ChipContext *ctx)
{
    ctx->SP--;                     // decrement from stack
    ctx->PC = ctx->stack[ctx->SP]; // pop value
}

/* Jump: Set PC to n1n2n3 */
void op_1NNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3)
{
    ctx->PC = (n1 << 8) | (n2 << 4) | n3;
}

/* Call: Call subroutine at memory location n1n2n3. */
void op_2NNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3)
{
    ctx->stack[ctx->SP] = ctx->PC; // push current instruction to stack
    ctx->SP++;                     // increment stack pointer

    ctx->PC = (n1 << 8) | (n2 << 4) | n3;
}

/* Skip if Vx == n1n2. */
void op_3XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2)
{
    if (ctx->V[x] == ((n1 << 4) | n2))
    {
        ctx->PC += 2;
    }
}

/* Skip if Vx != n1n2. */
void op_4XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2)
{
    if (ctx->V[x] != ((n1 << 4) | n2))
    {
        ctx->PC += 2;
    }
}

/* Skip if Vx == Vy. */
void op_5XY0(ChipContext *ctx, u8 x, u8 y)
{
    if (ctx->V[x] == ctx->V[y])
    {
        ctx->PC += 2;
    }
}

/* Skip if Vx != Vy. */
void op_9XY0(ChipContext *ctx, u8 x, u8 y)
{
    if (ctx->V[x] != ctx->V[y])
    {
        ctx->PC += 2;
    }
}

/* Vx <- n1n2. */
void op_6XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2)
{
    ctx->V[x] = (n1 << 4) | n2;
}

/* Vx += n1n2. */
void op_7XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2)
{
    // the flag register vF is NOT affected
    ctx->V[x] += (n1 << 4) | n2;
}

/* Vx <- Vy. */
void op_8XY0(ChipContext *ctx, u8 x, u8 y)
{
    ctx->V[x] = ctx->V[y];
}

/* Vx <- Vx OR Vy. */
void op_8XY1(ChipContext *ctx, u8 x, u8 y)
{
    ctx->V[x] |= ctx->V[y];
}

/* Vx <- Vx AND Vy. */
void op_8XY2(ChipContext *ctx, u8 x, u8 y)
{
    ctx->V[x] &= ctx->V[y];
}

/* Vx <- Vx XOR Vy. */
void op_8XY3(ChipContext *ctx, u8 x, u8 y)
{
    ctx->V[x] ^= ctx->V[y];
}

/* Vx <- Vx + Vy, this will affect the carry flag. */
void op_8XY4(ChipContext *ctx, u8 x, u8 y)
{
    u16 sum = ctx->V[x] + ctx->V[y];
    // mantain only the 8 LSB

    ctx->V[x] = sum & 0xFF;
    // if result is greater than 8 bits, update the carry flag
    if (sum > 0xFF)
    {
        ctx->V[0xF] = 1;
    }
    else
    {
        ctx->V[0xF] = 0;
    }
}

/* Vx <- Vx - Vy, this will affect the carry flag. */
void op_8XY5(ChipContext *ctx, u8 x, u8 y)
{
    u8 Vx = ctx->V[x];
    u8 Vy = ctx->V[y];

    u16 diff = (ctx->V[x] - ctx->V[y]) & 0xFF;
    ctx->V[x] = diff;

    if (Vx >= Vy)
    {
        ctx->V[0xF] = 1;
    }
    else
    {
        ctx->V[0xF] = 0;
    }
}

/* Vx <- Vy - Vx, this will affect the carry flag. */
void op_8XY7(ChipContext *ctx, u8 x, u8 y)
{
    u16 result = (ctx->V[y] - ctx->V[x]) & 0xFF;

    ctx->V[x] = result;

    if (ctx->V[y] >= ctx->V[x])
    {
        ctx->V[0xF] = 1;
    }
    else
    {
        ctx->V[0xF] = 0;
    }
}

/* Vx <- SHR Vx. */
void op_8XY6(ChipContext *ctx, u8 x, u8 y)
{
    // TODO: add via configuration mode this before Vx <- Vy

    u8 carry_bit = ctx->V[x] & 0x1;

    // store the LSB in VF
    ctx->V[x] >>= 1; // perform the operation
    ctx->V[0xF] = carry_bit;
}

/* Vx <- SHL Vx. */
void op_8XYE(ChipContext *ctx, u8 x, u8 y)
{
    // TODO: add via configuration mode this before Vx <- Vy

    u8 carry_bit = (ctx->V[x] & 0x80) >> 7;

    // store the MSB in VF
    ctx->V[x] = (ctx->V[x] << 1) & 0xFF; // perform the operation
    ctx->V[0xF] = carry_bit;
}

/* Sets the index register I to the value n1n2n3 */
void op_ANNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3)
{
    ctx->I = (n1 << 8) | (n2 << 4) | n3;
}

/* Jump with offset: Jump to the address n1n2n3 + V0 */
void op_BNNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3)
{
    ctx->PC = ctx->V[0] + ((n1 << 8) | (n2 << 4) | n3);
}

/* Vx <- rand() & n1n2 */
void op_CXNN(ChipContext *ctx, u8 x, u8 n1, u8 n2)
{
    u8 byte = (n1 << 4) | n2;

    ctx->V[x] = rand() & byte;
}

/*
Draw an N pixels tall sprite from the memory location that the I index register is holding to
the screen, at the horizontal X coordinate in Vx and the Y coordinate in Vy.
*/
void op_DXYN(ChipContext *ctx, u8 x, u8 y, u8 n)
{

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

/* Skip the following instruction if key[Vx] is pressed. */
void op_EX9E(ChipContext *ctx, u8 x)
{

    u8 key = ctx->V[x];

    if (ctx->keypad[key] == 1)
    {
        ctx->PC += 2;
    }

    return;
}

/* Skip the following instruction if key[Vx] is not pressed. */
void op_EXA1(ChipContext *ctx, u8 x)
{

    u8 key = ctx->V[x];

    if (ctx->keypad[key] == 0)
    {
        ctx->PC += 2;
    }
}

/* Vx <- delay_reg. */
void op_FX07(ChipContext *ctx, u8 x)
{
    ctx->V[x] = ctx->delay_reg;
}

/* delay_reg <- Vx. */
void op_FX15(ChipContext *ctx, u8 x)
{
    ctx->delay_reg = ctx->V[x];
}

/* sound_reg <- Vx. */
void op_FX18(ChipContext *ctx, u8 x)
{
    ctx->sound_reg = ctx->V[x];
}

/* I += Vx. */
void op_FX1E(ChipContext *ctx, u8 x)
{
    ctx->I = (ctx->I + ctx->V[x]) & 0xFFF; // keep between 12 bits
}

// FIXME: FX0A not working correctly on test 4
/* Get key: stop executing instructions and wait for key input. */
void op_FX0A(ChipContext *ctx, u8 x)
{
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

/* Font character: I <- address of the hex char in Vx.  */
void op_FX29(ChipContext *ctx, u8 x)
{
    ctx->I = FONTSET_START_ADDRESS + (5 * ctx->V[x]);
}

/* Binary-coded decimal conversion. */
void op_FX33(ChipContext *ctx, u8 x)
{
    u8 number = ctx->V[x];

    // hundreds
    ctx->memory[ctx->I] = number / 100;

    // tens
    ctx->memory[ctx->I + 1] = (number / 10) % 10;

    // unit
    ctx->memory[ctx->I + 2] = number % 10;
}

/* Store memory. Does not change the value of I. */
void op_FX55(ChipContext *ctx, u8 x)
{
    for (u8 i = 0; i <= x; i++)
    {
        ctx->memory[ctx->I + i] = ctx->V[i];
    }
}

/* Load memory. Does not change the value of I. */
void op_FX65(ChipContext *ctx, u8 x)
{
    for (u8 i = 0; i <= x; i++)
    {
        ctx->V[i] = ctx->memory[ctx->I + i];
    }
}

// Fetch an opcode from the ROM loaded in memory and decode it.
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

// Execute an instruction based on the opcode.
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
