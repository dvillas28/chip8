#include "utils.h"

void print_char(char c)
{
    printf("%c", c);
    fflush(stdout);
}

void print_mem(ChipContext *ctx)
{
    for (int i = 0; i < 4096; i++)
    {
        printf("Mem[%x]: %x\n", i, ctx->memory[i]);
    }
}

void print_regs(ChipContext *ctx)
{
    for (int i = 0; i < 16; i++)
    {
        printf("V%x: %x | ", i, ctx->V[i]);
    }
    printf("\n");
}

void print_keys(ChipContext *ctx)
{
    for (int i = 0; i < 16; i++)
    {
        printf("%d ", ctx->keypad[i]);
    }
    printf("\n");
}