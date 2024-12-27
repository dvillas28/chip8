#include "context.h"

const int RAM_SIZE = KB(4);

const u16 FONTSET_START_ADDRESS = 0x50;
const int FONTSET_SIZE = 80;

const u16 START_ADDRESS = 0x200;

void init_ctx(ChipContext *ctx)
{
    // write zeroes in memory
    for (int i = 0; i < RAM_SIZE; i++)
    {
        ctx->memory[i] = 0x00;
    }

    // write the fonts 050-09F
    load_fonts(ctx);

    // starts registers to zero
    for (int i = 0; i < 16; i++)
    {
        ctx->V[i] = 0x00;
        ctx->stack[i] = 0x0000;
        ctx->keypad[i] = 0;
    }

    ctx->PC = START_ADDRESS;
    ctx->I = 0x0000;
    ctx->SP = 0x00;
    ctx->delay_reg = 0x00;
    ctx->sound_reg = 0x00;

    // set display black
    for (int i = 0; i < CTX_WIDTH * CTX_HEIGHT; i++)
    {
        ctx->display[i] = 0x00;
    }
}

void load_fonts(ChipContext *ctx)
{

    u8 fonts[80] = {

        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        0xF0, 0x80, 0xF0, 0x80, 0x80};

    // remember that i is a pointer (12-bits)
    int j = 0;
    for (u16 i = 0; i < FONTSET_SIZE; i++)
    {
        ctx->memory[FONTSET_START_ADDRESS + i] = fonts[j];
        j++;
    }
}

int load_rom(ChipContext *cxt, char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }

    // read the contents to a buffer
    u8 buffer;
    int i = 0;

    while (fscanf(file, "%c", &buffer) == 1)
    {

        // printf("%x\n", buffer);
        cxt->memory[START_ADDRESS + i] = buffer;
        i++;
    }

    fclose(file);
    printf("ROM loaded sucessfully\n");
    return 0;
}