#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef char                    char8_t;
typedef float                   float32_t;
typedef double                  float64_t;

#define MAX_SIZE                4096
#define DEBUT_PROGRAM           0x200
#define DEBUT_ETI_660_PROGRAM   0X600
#define MAX_STACK               16
#define MAX_REGISTER               16
#define MAX_PIXELS              64*32
#define HIGH_BIT_SHIFT          >> 12
#define MAX_INSTRCUTION_SET     16
#define MAX_FONT                16 * 5
#define MAX_KEYPAD              16

typedef struct Instruction {
    uint8_t  opcode_id;
    uint8_t  X ;
    uint8_t  Y ;
    uint16_t NNN ;
    uint8_t  NN;
    uint8_t  N ;

}Instruction_t;

#define DISPLAY_INSTRUCTION(INS){\
    printf("-------------\n");\
\
    printf("----ID : 0X%01X\n", INS.opcode_id);\
    printf("----NNN : 0X%03X\n", INS.NNN);\
    printf("----NN : 0X%02X\n", INS.NN);\
\
    printf("----N : 0X%01X\n", INS.N);\
    printf("----X : 0X%01X\n", INS.X);\
    printf("----Y : 0X%01X\n", INS.Y);\
}\

typedef struct Chip_8 {
    uint8_t   MEMORY[MAX_SIZE];         // 4KB RAM
    uint16_t  STACK[MAX_STACK];         // STACK 16 x 16-bit
    uint8_t   V[MAX_REGISTER];          // 16 bytes register
    uint8_t   FRAME_BUFFER[MAX_PIXELS]; // FRAME BUFFER
    uint8_t   KEYPAD[MAX_KEYPAD];       // KEYCODE
    uint16_t  PC;                       // 16-bit REGISTER PROGRAM COUNTER
    uint16_t  INDEX;                    // 16-bit REGISTER INDEX
    uint8_t   DELAY_TIMER;              // 8-bit REGISTER DELAY TIMER
    uint8_t   SOUND_TIMER;              // 8-bit REGISTER SOUND TIMER
    uint8_t   SP;                       // 8-bit REGISTER STACK POINTER
}Chip_8_t;

void _chip_init_(Chip_8_t* cpu)
{
    srand(time(NULL));

    memset(cpu->MEMORY, 0, MAX_SIZE * sizeof(uint16_t));
    memset(cpu->STACK, 0, MAX_STACK * sizeof(uint16_t));
    memset(cpu->V, 0, MAX_REGISTER * sizeof(uint8_t));
    memset(cpu->FRAME_BUFFER, 0, MAX_PIXELS * sizeof(uint8_t));
    memset(cpu->KEYPAD, 0, MAX_KEYPAD * sizeof(uint8_t));

    cpu->INDEX        = 0;
    cpu->DELAY_TIMER  = 0;
    cpu->SOUND_TIMER  = 0;
    cpu->PC           = DEBUT_PROGRAM; // START OF ANY CODE
    cpu->SP           = 0;

    const uint8_t  FONT[MAX_FONT]  =  {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };

    memcpy(cpu->MEMORY, FONT, MAX_FONT * sizeof(uint8_t));
}

void _chip_decode(const uint16_t opcode, Instruction_t* nibles)
{
    (*nibles) = (Instruction_t){
        .opcode_id = (opcode & 0XF000) >> 12,
        .X         = (opcode & 0X0F00) >> 8,
        .Y         = (opcode & 0X00F0) >> 4,
        .NNN       = (opcode & 0X0FFF),
        .NN        = (opcode & 0X00FF) ,
        .N         = (opcode & 0X000F)
    };
}

void _chip_process_ins(Chip_8_t* cpu, const uint16_t opcode)
{
    Instruction_t n;

    _chip_decode(opcode, &n);

    static uint16_t result ;
    static uint8_t  random_number;
    static uint8_t  result8_t;
    
    switch (n.opcode_id)
    {
    case 0x0:
        
        switch (n.NNN)
        {
        case 0x0E0: // CLS
            memset(cpu->FRAME_BUFFER, 0, MAX_PIXELS * sizeof(uint8_t));
            break;

        case 0x0EE: // RET
            cpu->PC  = cpu->STACK[cpu->SP];
            cpu->SP -= 1;
            break;
        
        default:
            break;
        }
        
        break;
    
    case 0x1: // JP NNN
        cpu->PC = n.NNN;
        break;

    case 0x2: // CALL NNN
        cpu->SP            += 1;
        cpu->STACK[cpu->SP] = cpu->PC;
        cpu->PC             = n.NNN;
        break;

    case 0x3: // SE VX, byte
        cpu->PC = cpu->V[n.X] == n.NN ? cpu->PC + 2 : cpu->PC;
        break;
    
    case 0x4: // SNE VX, byte
        cpu->PC = cpu->V[n.X] != n.NN ? cpu->PC + 2 : cpu->PC;
        break;

    case 0x5: // SE VX, VY
        if(n.N == 0)
            cpu->PC = cpu->V[n.X] != cpu->V[n.Y] ? cpu->PC + 2 : cpu->PC;
        break;

    case 0x6:
        cpu->V[n.X] = n.NN;
        break;
    
    case 0x7:
        cpu->V[n.X] += n.NN;
        break;
    
    case 0x8:
        switch (n.N)
        {
        case 0x0:
            cpu->V[n.X] = cpu->V[n.Y];
            break;
        case 0x1:
        
            cpu->V[n.X] |= cpu->V[n.Y];
            break;

        case 0x2:
            cpu->V[n.X] &= cpu->V[n.Y];
            break;

        case 0x3:
            cpu->V[n.X] ^= cpu->V[n.Y];
            break;

        case 0x4:
            result               = cpu->V[n.X] + cpu->V[n.Y];
            cpu->V[0xF] = result > 0xFF ? 1 : 0;
            cpu->V[n.X] = result & 0x0F;
            break;

        case 0x5:
            cpu->V[0xF] = cpu->V[n.X] > cpu->V[n.Y] ? 1 : 0;
            result               = cpu->V[n.X] - cpu->V[n.Y];
            cpu->V[n.X] = result & 0x0F;
            break;

        case 0x6:
            cpu->V[0xF] = cpu->V[n.X] & 0x01;
            result               = cpu->V[n.X] / 2;
            cpu->V[n.X] = result & 0x0F;
            break;

        case 0x7:
            cpu->V[0xF] = cpu->V[n.X] < cpu->V[n.Y];
            result               = cpu->V[n.Y] - cpu->V[n.X];
            cpu->V[n.X] = result & 0x0F;
            break;

        case 0xE:
            cpu->V[0xF] = (cpu->V[n.X] & 0x80) >> 7;
            result               = cpu->V[n.X] / 2;
            cpu->V[n.X] = result & 0x0F;            
            break;

        default:
            break;
        }

        break;

    case 0x9:
        if(n.N == 0)
            cpu->PC = cpu->V[n.X] != cpu->V[n.Y] ? cpu->PC + 2 : cpu->PC;
        break;

    case 0xA:
        cpu->INDEX     = n.NNN;
        break;

    case 0xB:
        cpu->PC    = n.NNN + cpu->V[0x0];
        break;

    case 0xC:
        random_number        = (uint8_t)rand() % 256;
        result8_t            = random_number & n.NN;
        cpu->V[n.X] = result8_t;
        break;

    case 0xD:
        cpu->V[0xF] = 0;
        uint8_t xPos = cpu->V[n.X] & 63;
        uint8_t yPos = cpu->V[n.Y] & 31;

        for(uint8_t spriteRow = 0; spriteRow < n.N; ++ spriteRow)
        {
            uint8_t spriteNByte = cpu->MEMORY[cpu->INDEX + spriteRow];
            
            for(uint8_t spriteBit = 0; spriteBit < 8; ++ spriteBit)
            {
               if(spriteNByte & ( 1 << spriteBit) && cpu->FRAME_BUFFER[xPos * 64 + yPos])
                    cpu->V[0xF] = 1;
               
               if(spriteNByte & ( 1 << spriteBit) && !cpu->FRAME_BUFFER[xPos * 64 + yPos])
                    cpu->FRAME_BUFFER[xPos * 64 + yPos] ^= spriteNByte & ( 1 << spriteBit);

                if(xPos >= 64)
                    break;
                ++xPos;
            }
                ++yPos;
                if(yPos >= 32)
                    break;
        }
        break;

    case 0xE:
        switch (n.NN)
        {
        case 0x9E:
            cpu->PC = cpu->KEYPAD[cpu->V[n.X]] ? cpu->PC + 2 : cpu->PC;
            break;

        case 0xA1:
            cpu->PC = !cpu->KEYPAD[cpu->V[n.X]] ? cpu->PC + 2 : cpu->PC;
            break;

        default:
            break;
        }
        
        break;

    case 0xF:
        switch (n.NN)
        {
        case 0x07:
            cpu->V[n.X] = cpu->DELAY_TIMER;
            break;

        case 0x0A:
            //TODO
            break;

        case 0x15:
            cpu->DELAY_TIMER = cpu->V[n.X];
            break;

        case 0x18:
            cpu->SOUND_TIMER = cpu->V[n.X];
            break;

        case 0x1E:
            cpu->INDEX += cpu->V[n.X]; 
            break;

        case 0x29:
            cpu->INDEX = cpu->MEMORY[cpu->V[n.X] * 5];
            break;

        case 0x33:
            cpu->MEMORY[cpu->INDEX]     = (cpu->V[n.X] & 0x04) >> 2;
            cpu->MEMORY[cpu->INDEX + 1] = (cpu->V[n.X] & 0x02) >> 1;
            cpu->MEMORY[cpu->INDEX + 2] = (cpu->V[n.X] & 0x01);
            break;

        case 0x55: // LD [I], VX
            memcpy(cpu->MEMORY + cpu->INDEX, cpu->V, (n.X +1)* sizeof(uint8_t));
            break;

        case 0x65: // LD VX, [1]
            memcpy(cpu->V, cpu->MEMORY + cpu->INDEX, (n.X +1) * sizeof(uint8_t));
            break;

        default:
            
            break;
        }
        break;

    default:
        break;
    }
}


#endif