#ifndef CHIP_8_H
#define CHIP_8_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "instruction_confg.h"

#define MEMORY_SIZE       4096
#define MAX_VALUE         16
#define START_PROGRAM     0X200
#define INSTRUCTION_SET_SIZE 0XFFFF

typedef struct Chip8
{
    uint8_t       RAM[MEMORY_SIZE];                 //   4kb (4096) of RAM
    uint8_t       V[MAX_VALUE];                     //   16 V register from 0 to F. (Disclaimer! VF is only used as flags by some instruction)
    uint16_t      I;                                //   Index register (only the lowest 12-bit is used)
    uint8_t       SOUND_TIMER;                      //   Sound timer register decrements until it reach 0
    uint8_t       DELAY_TIMER;                      //   Delay timer register decrements until it reach 0
    uint8_t       KEYBOARD[MAX_VALUE];              //   Keyboard values set to 0 (pressed) or 1 (unpressed)
    uint16_t      PC;                               //   Program counter register used to fetch data and other...
    uint8_t       SP;                               //   Stack pointer register keep track of top of the stack
    uint8_t       STACK[MAX_VALUE];                 //   Stack store the address that the interpreter should return with a subroutine
    Config_Instruction_t CURRENT_INSTRUCTION;              //   Current instruction
}Chip8_t;

#include "instruction.h"


extern void(*INSTRUCTION_SET[INSTRUCTION_SET_SIZE])(Chip8_t*);

void Chip8_init               (Chip8_t* c);
void Chip8_load_rom           (Chip8_t* c, const char* filepath);
void Chip8_fetch              (Chip8_t* c);
void Chip8_process_instruction(Chip8_t* c);

#endif