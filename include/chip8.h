#ifndef CHIP_8_H
#define CHIP_8_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>

#define MEMORY_SIZE          4096
#define MAX_VALUE            16
#define START_PROGRAM        0X200
#define INSTRUCTION_SET_SIZE 0XFFFF
#define RESOLUTION           64*32

#define RED      "\x1B[31m"
#define GREEN    "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE     "\x1B[34m"
#define WHITE    "\x1B[37m"
#define RESET    "\x1B[0m"

#define _X   c->CURRENT_INSTRUCTION.X
#define _Y   c->CURRENT_INSTRUCTION.Y
#define _N   c->CURRENT_INSTRUCTION.N
#define _NN  c->CURRENT_INSTRUCTION.NN
#define _NNN c->CURRENT_INSTRUCTION.NNN
#define _ID  c->CURRENT_INSTRUCTION.ID
#define _OPCODE  c->CURRENT_INSTRUCTION.OPCODE

typedef enum Extention
{
   CHIP8 = 0,
   SUPER_CHIP8
}Extention;

typedef struct Bit_mask
{

   uint8_t  INDEX;
   uint16_t OFFSET;

}Bit_mask_t;

typedef struct Config_Instruction
{

   uint16_t OPCODE; // 16-bit  Instruction  opcode
   uint16_t NNN;    // lowest  12-bit of opcode
   uint16_t NN;     // lowest  8-bits of opcode
   uint8_t  N;      // lowest  4-bits of opcode
   uint8_t  ID;
   uint8_t  X;      // lowest  4-bits of the high bits of opcode
   uint8_t  Y;      // highest 4-bits of the low bits of opcode

}Config_Instruction_t;

typedef struct Chip8
{
    uint8_t              RAM[MEMORY_SIZE];                 //   4kb (4096) of RAM
    uint8_t              V[MAX_VALUE];                     //   16 V register from 0 to F. (Disclaimer! VF is only used as flags by some instruction)
    uint16_t             I;                                //   Index register (only the lowest 12-bit is used)
    uint8_t              SOUND_TIMER;                      //   Sound timer register decrements until it reach 0
    uint8_t              DELAY_TIMER;                      //   Delay timer register decrements until it reach 0
    uint8_t              KEYBOARD[MAX_VALUE];              //   Keyboard values set to 0 (pressed) or 1 (unpressed)
    uint16_t             PC;                               //   Program counter register used to fetch data and other...
    uint8_t              SP;                               //   Stack pointer register keep track of top of the stack
    uint8_t              PIXELS[RESOLUTION];               //   Screen, each pixel state is either 0 or 1.
    uint16_t              STACK[MAX_VALUE];                 //   Stack store the address that the interpreter should return with a subroutine
    Config_Instruction_t CURRENT_INSTRUCTION;              //   Current instruction
    uint8_t              KEY_PRESSED;
    Extention            EXTENTION;
}Chip8_t;


typedef struct Rendering
{
   int32_t width;
   int32_t height;
   uint8_t hpixel;
   uint8_t wpixel;
   uint8_t scale;

   SDL_Window* window;
   SDL_Renderer* render;

   SDL_Event event;

   uint8_t isExit;

   char name[20];
}Rendering;


extern Bit_mask_t BIT_MAP[0x10];
extern void(*INSTRUCTION_SET[INSTRUCTION_SET_SIZE])(Chip8_t*);


void Instruction_decode(Config_Instruction_t* instruction, const uint16_t opcode);

void Instruction_0NNN_SYS(Chip8_t* c);
void Instruction_00E0_CLS(Chip8_t* c);
void Instruction_00EE_RET(Chip8_t* c);
void Instruction_1NNN_JP(Chip8_t* c);
void Instruction_2NNN_CALL(Chip8_t* c);
void Instruction_3XNN_SE(Chip8_t* c);
void Instruction_4XNN_SNE(Chip8_t* c);

void Instruction_5XY0_SE(Chip8_t* c);
void Instruction_6XNN_LD(Chip8_t* c);
void Instruction_7XNN_ADD(Chip8_t* c);
void Instruction_8XY0_LD(Chip8_t* c);
void Instruction_8XY1_OR(Chip8_t* c);
void Instruction_8XY2_AND(Chip8_t* c);

void Instruction_8XY3_XOR(Chip8_t* c);
void Instruction_8XY4_ADD(Chip8_t* c);
void Instruction_8XY5_SUB(Chip8_t* c);
void Instruction_8XY6_SHR(Chip8_t* c);
void Instruction_8XY7_SUBN(Chip8_t* c);
void Instruction_8XYE_SHL(Chip8_t* c);

void Instruction_9XY0_SNE(Chip8_t* c);
void Instruction_ANNN_LD(Chip8_t* c);
void Instruction_BNNN_AND(Chip8_t* c);
void Instruction_CXNN_AND(Chip8_t* c);
void Instruction_DXYN_DRW(Chip8_t* c);
void Instruction_EX9E_SKP(Chip8_t* c);

void Instruction_EXA1_SKNP(Chip8_t* c);
void Instruction_FX07_LD(Chip8_t* c);
void Instruction_FX0A_LD(Chip8_t* c);
void Instruction_FX15_LD(Chip8_t* c);
void Instruction_FX18_LD(Chip8_t* c);
void Instruction_FX1E_ADD(Chip8_t* c);

void Instruction_FX29_LD(Chip8_t* c);
void Instruction_FX33_LD(Chip8_t* c);
void Instruction_FX55_LD(Chip8_t* c);
void Instruction_FX65_LD(Chip8_t* c);

void chip8_init               (Chip8_t* c);
void chip8_load_rom           (Chip8_t* c, const char* filepath);
void chip8_fetch              (Chip8_t* c);
void chip8_process_instruction(Chip8_t* c);

void render_init(Rendering* screen);
void process_keyboard(Rendering* screen, Chip8_t* c);
void loop(Rendering* screen, Chip8_t* c);
void clear_render(Rendering* screen);


#endif