/* cpu.h
 * CPU header
 */

#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

// TODO: use this struct
typedef struct
{
  uint8_t opcode;           // Opcode value
  char* disassembly;        // Opcode mnemonic
  uint8_t operand_len;      // Number of operands
} instruction;

// TODO: add interrupt information
typedef struct
{
    union
    {
        struct
        {
            uint8_t C;
            uint8_t B;
        };
        uint16_t BC;
    };
    union
    {
        struct
        {
            uint8_t E;
            uint8_t D;
        };
        uint16_t DE;
    };
    union
    {
        struct
        {
            uint8_t L;
            uint8_t H;
        };
        uint16_t HL;
    };

    // A is the accumulator register and F is the flags register
    union
    {
        struct
        {
            union
            {
                struct
                {
                    uint8_t unused : 4;
                    uint8_t C      : 1;
                    uint8_t H      : 1;
                    uint8_t N      : 1;
                    uint8_t Z      : 1;
                } flags;
                uint8_t F;
            };
            uint8_t A;
        };
        uint16_t AF;
    };
    uint16_t SP; // Stack pointer
    uint16_t PC; // Program counter
    uint8_t IME; // Interrupt Master Enable flag
} CPU;

CPU* cpu_init(void);
void cpu_tick(CPU* cpu);

#endif /* _CPU_H_ */
