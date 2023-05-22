/* cpu.c
 *
 * LR35902 CPU emulation implementation
 */

#include "cpu.h"
#include "mem.h"
#include "interrupt.h"
#include "instructions.h"
#include <cstdint>
#include <stdlib.h>

static uint8_t fetch(CPU* cpu) {
    return mem_read(cpu->PC);
}

static Instruction decode(uint8_t opcode) {
    // TODO: Use the algorithmic instruction decoding approach
    Instruction inst;
    inst.opcode = opcode;
    return inst;
}

static void execute(CPU* cpu, Instruction inst) {
    // TODO: Execute the command and update flags
    switch(inst.opcode) {
    }
}

CPU* cpu_init(void) {
    CPU* cpu = (CPU*)malloc(sizeof(CPU));
    cpu_reset(cpu);
    return cpu;
}

void cpu_reset(CPU* cpu) {
    cpu->SP = 0xFFFE;
    cpu->PC = 0x0100;
}

void cpu_tick(CPU* cpu) {
    uint8_t opcode = fetch(cpu);
    Instruction inst = decode(opcode);
    execute(cpu, inst);
}
