#pragma once
#include <stdio.h>
#include "types.h"

#define CTX_WIDTH 64
#define CTX_HEIGHT 32

#define CTX_TICKS_PER_SECOND 600
#define CTX_DELAY 3
#define CTX_TICKS_TIMER_PER_SECOND 60

typedef struct ChipContext
{
    // 4KB = 4096B = 2^12B of memory
    u8 memory[KB(4)];

    // 16 8-bit general purpose variable registers 0-F
    u8 V[16];

    u16 stack[16]; // stack for 16-bit addresses

    // Special registers
    u16 PC;       // PC (program counter) 12-bit (16-bit in theory)
    u16 I;        // I (index register) 12-bit (16-bit in theory)
    u8 SP;        // stack pointer
    u8 delay_reg; // Delay Timer 8-bit
    u8 sound_reg; // Sound Timer 8-bit

    // Display 64 x 32 pixels, monochrome
    b8 display[CTX_WIDTH * CTX_HEIGHT];

    // keys
    b8 keypad[16];

} ChipContext;

void init_ctx(ChipContext *ctx);
void load_fonts(ChipContext *ctx);
int load_rom(ChipContext *ctx, char *filename);