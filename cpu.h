/* cpu.h
 * CPU header
 */

#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

typedef struct
{
  uint8_t opcode;           /* Opcode value */
  char* disassembly;        /* Opcode mnemonic */
  uint8_t operand_len;      /* Number of operands */
} instruction;

typedef struct
{
    /* A is the accumulator register and F is the flags register */
    uint16_t AF, BC, DE, HL;  /* General purpose registers */
    uint16_t SP;              /* Stack pointer */
    uint16_t PC;              /* Program counter */

} CPU;

void init(CPU* cpu);
void run(CPU* cpu);

#endif /* _CPU_H_ */
