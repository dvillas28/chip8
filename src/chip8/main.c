#include <stdlib.h>

#include "chip8.h"
#include "context.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s INPUT\n", argv[0]);
        return 1;
    }

    ChipContext ctx;
    init_ctx(&ctx);

    if (load_rom(&ctx, argv[1]) != 0)
    {
        printf("Could not read ROM\n");
    }
    else
    {
        start_chip8(&ctx);
    }
}