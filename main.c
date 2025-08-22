#include "include/chip8.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Chip8_t cpu;
    Chip8_init(&cpu);
    Chip8_load_rom(&cpu, "ROM/1-chip8-logo.ch8");

}