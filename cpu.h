/* cpu.h
 * CPU header
 */

#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

/* TODO: use this struct */
typedef struct
{
  uint8_t opcode;           /* Opcode value */
  char* disassembly;        /* Opcode mnemonic */
  uint8_t operand_len;      /* Number of operands */
} instruction;

typedef struct
{
    union
    {
        struct bytes
        {
            uint8_t lo;
            uint8_t hi;
        } bytes;
        uint16_t word;
    };
} gb_register;

typedef struct
{
    /* A is the accumulator register and F is the flags register */
    gb_register AF, BC, DE, HL;   /* General purpose registers */
    uint16_t SP;                  /* Stack pointer */
    uint16_t PC;                  /* Program counter */
} CPU;

void init(CPU* cpu);
void run(CPU* cpu);

#endif /* _CPU_H_ */
