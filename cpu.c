/* cpu.c
 * Implements the CPU
 */

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "mem.h"
#include "interrupt.h"

#define HI_NIBBLE 0xF0
#define LO_NIBBLE 0x0F
#define ZERO_PAGE_BASE 0xFF00

static inline void illegal(void)
{
    sprintf(stderr, "ERROR: Illegal opcode!");
    exit(1); /* Exited with error */
}

static inline void set_bit(uint8_t* byte, int pos)
{
    *byte |= (1 << pos);
}

static inline void clear_bit(uint8_t* byte, int pos)
{
    *byte &= ~(1 << pos);
}

static inline int test_bit(uint8_t* byte, int pos)
{
    return (*byte >> pos) & 1;
}

/* Initialize the CPU */
void cpu_init(CPU* cpu)
{
    cpu->AF = 0x01B0;
    cpu->BC = 0x0013;
    cpu->DE = 0x00D8;
    cpu->HL = 0x014D;
    cpu->SP = 0xFFFE;
    cpu->PC = 0x0100;
    cpu->IME = 0x00;
}

/* Run the CPU */
void cpu_tick(CPU* cpu)
{
    /* TODO: refactor this to account for the following:
     *          - the proper amount of cycles to run for
     *          - update flags where needed
     *          - algorithmically parse instruction encodings
     *            to increase efficiency of this switch statement
     */
    uint8_t inst = mem_read(cpu->PC);
    uint8_t temp_nibble = 0x00;     /* Nibble-size operand */
    uint16_t operandw = 0x0000;     /* Word-size operand */
    uint16_t push, call;  /* Word-sized operands */
    push = call = 0x0000;  /* Word-sized operands */
    switch(inst)
    {
        /* ------- Miscellaneous/control ------- */
        case 0x00:  /* NOP */
            /* Do nothing */
            break;
        case 0x10:  /* STOP 0 */
            break;
        case 0x76:  /* HALT */
            break;
        case 0xF3:  /* DI */
            interrupt_disable();
            break;
        case 0xFB:  /* EI */
            interrupt_enable();
            break;

        /* ------- Jumps/calls ------- */
        case 0x18:  /* JR r8 */
            cpu->PC += (int8_t)mem_read(cpu->PC+1);
            break;
        case 0x20:  /* JR NZ,r8 */
            if (!cpu->flags.Z)
                cpu->PC += (int8_t)mem_read(cpu->PC+1);
            break;
        case 0x28:  /* JR Z,r8 */
            if ( cpu->flags.Z)
                cpu->PC += (int8_t)mem_read(cpu->PC+1);
            break;
        case 0x30:  /* JR NC,r8 */
            if (!cpu->flags.C)
                cpu->PC += (int8_t)mem_read(cpu->PC+1);
            break;
        case 0x38:  /* JR C,r8 */
            if ( cpu->flags.C)
                cpu->PC += (int8_t)mem_read(cpu->PC+1);
            break;
        case 0xC0:  /* RET NZ */
            if (!cpu->flags.Z)
                cpu->PC = mem_read(cpu->SP++) | (mem_read(cpu->SP++) << 8);
            break;
        case 0xC2:  /* JP NZ,a16 */
            if (!cpu->flags.Z)
                cpu->PC = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            break;
        case 0xC3:  /* JP a16 */
            cpu->PC = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            break;
        case 0xC4:  /* CALL NZ,a16 */
            if (!cpu->flags.Z)
            {
                call = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
                push = cpu->PC+3;
                mem_write(--cpu->SP, (uint8_t)(push >> 8));
                mem_write(--cpu->SP, (uint8_t)(push & 0xFF));
                cpu->PC = call;
            }
            break;
        case 0xC7:  /* RST 00H */
            mem_write(--cpu->SP, (uint8_t)((cpu->PC) >> 8));
            mem_write(--cpu->SP, (uint8_t)(cpu->PC & 0xFF));
            cpu->PC = 0x0000;
            break;
        case 0xC8:  /* RET Z */
            if ( cpu->flags.Z)
                cpu->PC = mem_read(cpu->SP++) | (mem_read(cpu->SP++) << 8);
            break;
        case 0xC9:  /* RET */
            cpu->PC = mem_read(cpu->SP++) | (mem_read(cpu->SP++) << 8);
            break;
        case 0xCA:  /* JP Z,a16 */
            if ( cpu->flags.Z)
                cpu->PC = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            break;
        case 0xCC:  /* CALL Z,a16 */
            if ( cpu->flags.Z)
            {
                call = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
                push = cpu->PC+3;
                mem_write(--cpu->SP, (uint8_t)(push >> 8));
                mem_write(--cpu->SP, (uint8_t)(push & 0xFF));
                cpu->PC = call;
            }
            break;
        case 0xCD:  /* CALL a16 */
            call = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            push = cpu->PC+3;
            mem_write(--cpu->SP, (uint8_t)(push >> 8));
            mem_write(--cpu->SP, (uint8_t)(push & 0xFF));
            cpu->PC = call;
            break;
        case 0xCF:  /* RST 08H */
            mem_write(--cpu->SP, (uint8_t)((cpu->PC) >> 8));
            mem_write(--cpu->SP, (uint8_t)(cpu->PC & 0xFF));
            cpu->PC = 0x0008;
            break;
        case 0xD0:  /* RET NC */
            if (!cpu->flags.C)
                cpu->PC = mem_read(cpu->SP++) | (mem_read(cpu->SP++) << 8);
            break;
        case 0xD2:  /* JP NC,a16 */
            if (!cpu->flags.C)
                cpu->PC = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            break;
        case 0xD4:  /* CALL NC,a16 */
            if (!cpu->flags.C)
            {
                call = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
                push = cpu->PC+3;
                mem_write(--cpu->SP, (uint8_t)(push >> 8));
                mem_write(--cpu->SP, (uint8_t)(push & 0xFF));
                cpu->PC = call;
            }
            break;
        case 0xD7:  /* RST 10H */
            mem_write(--cpu->SP, (uint8_t)((cpu->PC) >> 8));
            mem_write(--cpu->SP, (uint8_t)(cpu->PC & 0xFF));
            cpu->PC = 0x0010;
            break;
        case 0xD8:  /* RET C */
            if ( cpu->flags.C)
                cpu->PC = mem_read(cpu->SP++) | (mem_read(cpu->SP++) << 8);
            break;
        case 0xD9:  /* RETI */
            cpu->PC = mem_read(cpu->SP++) | (mem_read(cpu->SP++) << 8);
            interrupt_enable();
            break;
        case 0xDA:  /* JP C,a16 */
            if ( cpu->flags.C)
                cpu->PC = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            break;
        case 0xDC:  /* CALL C,a16 */
            if ( cpu->flags.C)
            {
                call = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
                push = cpu->PC+3;
                mem_write(cpu->SP++, (uint8_t)(push >> 8));
                mem_write(cpu->SP++, (uint8_t)(push & 0xFF));
                cpu->PC = call;
            }
            break;
        case 0xDF:  /* RST 18H */
            mem_write(--cpu->SP, (uint8_t)((cpu->PC) >> 8));
            mem_write(--cpu->SP, (uint8_t)(cpu->PC & 0xFF));
            cpu->PC = 0x0018;
            break;
        case 0xE7:  /* RST 20H */
            mem_write(--cpu->SP, (uint8_t)((cpu->PC) >> 8));
            mem_write(--cpu->SP, (uint8_t)(cpu->PC & 0xFF));
            cpu->PC = 0x0020;
            break;
        case 0xE9:  /* JP (HL) */
            cpu->PC = cpu->HL;
            break;
        case 0xEF:  /* RST 28H */
            mem_write(--cpu->SP, (uint8_t)((cpu->PC) >> 8));
            mem_write(--cpu->SP, (uint8_t)(cpu->PC & 0xFF));
            cpu->PC = 0x0028;
            break;
        case 0xF7:  /* RST 30H */
            mem_write(--cpu->SP, (uint8_t)((cpu->PC) >> 8));
            mem_write(--cpu->SP, (uint8_t)(cpu->PC & 0xFF));
            cpu->PC = 0x0030;
            break;
        case 0xFF:  /* RST 38H */
            mem_write(--cpu->SP, (uint8_t)((cpu->PC) >> 8));
            mem_write(--cpu->SP, (uint8_t)(cpu->PC & 0xFF));
            cpu->PC = 0x0038;
            break;

        /* ------- 8-bit loads/stores ------- */
        case 0x02:  /* LD (BC),A */
            mem_write(cpu->BC, cpu->A);
            break;
        case 0x06:  /* LD B,d8 */
            cpu->B = mem_read(cpu->PC+1);
            break;
        case 0x0A:  /* LD A,(BC) */
            cpu->A = mem_read(cpu->BC);
            break;
        case 0x0E:  /* LD C,d8 */
            cpu->C = mem_read(cpu->PC+1);
            break;
        case 0x12:  /* LD (DE),A */
            mem_write(cpu->DE, cpu->A);
            break;
        case 0x16:  /* LD D,d8 */
            cpu->D = mem_read(cpu->PC+1);
            break;
        case 0x1A:  /* LD A,(DE) */
            cpu->A = mem_read(cpu->DE);
            break;
        case 0x1E:  /* LD E,d8 */
            cpu->DE = mem_read(cpu->PC+1);
            break;
        case 0x22:  /* LD (HL+),A */
            mem_write(cpu->HL++, cpu->A);
            break;
        case 0x26:  /* LD H,d8 */
            cpu->H = mem_read(cpu->PC+1);
            break;
        case 0x2A:  /* LD A,(HL+) */
            cpu->A = mem_read(cpu->HL++);
            break;
        case 0x2E:  /* LD L,d8 */
            cpu->L = mem_read(cpu->PC+1);
            break;
        case 0x32:  /* LD (HL-),A */
            mem_write(cpu->HL--, cpu->A);
            break;
        case 0x36:  /* LD (HL),d8 */
            mem_write(cpu->HL, mem_read(cpu->PC+1));
            break;
        case 0x3A:  /* LD A,(HL-) */
            cpu->A = mem_read(cpu->HL--);
            break;
        case 0x3E:  /* LD A,d8 */
            cpu->A = mem_read(cpu->PC+1);
            break;
        case 0x40:  /* LD B,B */
            cpu->B = cpu->B;
            break;
        case 0x41:  /* LD B,C */
            cpu->B = cpu->C;
            break;
        case 0x42:  /* LD B,D */
            cpu->B = cpu->D;
            break;
        case 0x43:  /* LD B,E */
            cpu->B = cpu->E;
            break;
        case 0x44:  /* LD B,H */
            cpu->B = cpu->H;
            break;
        case 0x45:  /* LD B,L */
            cpu->B = cpu->L;
            break;
        case 0x46:  /* LD B,(HL) */
            cpu->B = mem_read(cpu->HL);
            break;
        case 0x47:  /* LD B,A */
            cpu->B = cpu->A;
            break;
        case 0x48:  /* LD C,B */
            cpu->C = cpu->B;
            break;
        case 0x49:  /* LD C,C */
            cpu->C = cpu->C;
            break;
        case 0x4A:  /* LD C,D */
            cpu->C = cpu->D;
            break;
        case 0x4B:  /* LD C,E */
            cpu->C = cpu->E;
            break;
        case 0x4C:  /* LD C,H */
            cpu->C = cpu->H;
            break;
        case 0x4D:  /* LD C,L */
            cpu->C = cpu->L;
            break;
        case 0x4E:  /* LD C,(HL) */
            cpu->C = mem_read(cpu->HL);
            break;
        case 0x4F:  /* LD C,A */
            cpu->C = cpu->A;
            break;
        case 0x50:  /* LD D,B */
            cpu->D = cpu->B;
            break;
        case 0x51:  /* LD D,C */
            cpu->D = cpu->C;
            break;
        case 0x52:  /* LD D,D */
            cpu->D = cpu->D;
            break;
        case 0x53:  /* LD D,E */
            cpu->D = cpu->E;
            break;
        case 0x54:  /* LD D,H */
            cpu->D = cpu->H;
            break;
        case 0x55:  /* LD D,L */
            cpu->D = cpu->L;
            break;
        case 0x56:  /* LD D,(HL) */
            cpu->D = mem_read(cpu->HL);
            break;
        case 0x57:  /* LD D,A */
            cpu->D = cpu->A;
            break;
        case 0x58:  /* LD E,B */
            cpu->E = cpu->B;
            break;
        case 0x59:  /* LD E,C */
            cpu->E = cpu->C;
            break;
        case 0x5A:  /* LD E,D */
            cpu->E = cpu->D;
            break;
        case 0x5B:  /* LD E,E */
            cpu->E = cpu->E;
            break;
        case 0x5C:  /* LD E,H */
            cpu->E = cpu->H;
            break;
        case 0x5D:  /* LD E,L */
            cpu->E = cpu->L;
            break;
        case 0x5E:  /* LD E,(HL) */
            cpu->E = mem_read(cpu->HL);
            break;
        case 0x5F:  /* LD E,A */
            cpu->E = cpu->A;
            break;
        case 0x60:  /* LD H,B */
            cpu->H = cpu->B;
            break;
        case 0x61:  /* LD H,C */
            cpu->H = cpu->C;
            break;
        case 0x62:  /* LD H,D */
            cpu->H = cpu->D;
            break;
        case 0x63:  /* LD H,E */
            cpu->H = cpu->E;
            break;
        case 0x64:  /* LD H,H */
            cpu->H = cpu->H;
            break;
        case 0x65:  /* LD H,L */
            cpu->H = cpu->L;
            break;
        case 0x66:  /* LD H,(HL) */
            cpu->H = mem_read(cpu->HL);
            break;
        case 0x67:  /* LD H,A */
            cpu->H = cpu->A;
            break;
        case 0x68:  /* LD L,B */
            cpu->L = cpu->B;
            break;
        case 0x69:  /* LD L,C */
            cpu->L = cpu->C;
            break;
        case 0x6A:  /* LD L,D */
            cpu->L = cpu->D;
            break;
        case 0x6B:  /* LD L,E */
            cpu->L = cpu->E;
            break;
        case 0x6C:  /* LD L,H */
            cpu->L = cpu->H;
            break;
        case 0x6D:  /* LD L,L */
            cpu->L = cpu->L;
            break;
        case 0x6E:  /* LD L,(HL) */
            cpu->L = mem_read(cpu->HL);
            break;
        case 0x6F:  /* LD L,A */
            cpu->L = cpu->A;
            break;
        case 0x70:  /* LD (HL),B */
            mem_write(cpu->HL, cpu->B);
            break;
        case 0x71:  /* LD (HL),C */
            mem_write(cpu->HL, cpu->C);
            break;
        case 0x72:  /* LD (HL),D */
            mem_write(cpu->HL, cpu->D);
            break;
        case 0x73:  /* LD (HL),E */
            mem_write(cpu->HL, cpu->E);
            break;
        case 0x74:  /* LD (HL),H */
            mem_write(cpu->HL, cpu->H);
            break;
        case 0x75:  /* LD (HL),L */
            mem_write(cpu->HL, cpu->L);
            break;
        case 0x77:  /* LD (HL),A */
            mem_write(cpu->HL, cpu->A);
            break;
        case 0x78:  /* LD A,B */
            cpu->A = cpu->B;
            break;
        case 0x79:  /* LD A,C */
            cpu->A = cpu->C;
            break;
        case 0x7A:  /* LD A,D */
            cpu->A = cpu->D;
            break;
        case 0x7B:  /* LD A,E */
            cpu->A = cpu->E;
            break;
        case 0x7C:  /* LD A,H */
            cpu->A = cpu->H;
            break;
        case 0x7D:  /* LD A,L */
            cpu->A = cpu->L;
            break;
        case 0x7E:  /* LD A,(HL) */
            cpu->A = mem_read(cpu->HL);
            break;
        case 0x7F:  /* LD A,A */
            cpu->A = cpu->A;
            break;
        case 0xE0:  /* LDH (a8),A */
            mem_write((ZERO_PAGE_BASE + mem_read(cpu->PC+1)), cpu->A);
            break;
        case 0xE2:  /* LD (C),A */
            mem_write((ZERO_PAGE_BASE + cpu->C), cpu->A);
            break;
        case 0xEA:  /* LD (a16),A */
            operandw = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            mem_write(operandw, cpu->A);
            break;
        case 0xF0:  /* LDH A,(a8) */
            cpu->A = mem_read((ZERO_PAGE_BASE + mem_read(cpu->PC+1)));
            break;
        case 0xF2:  /* LD A,(C) */
            cpu->A = mem_read((ZERO_PAGE_BASE + cpu->C));
            break;
        case 0xFA:  /* LD A,(a16) */
            operandw = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            cpu->A = mem_read(operandw);
            break;

        /* ------- 16-bit loads/stores ------- */
        case 0x01:  /* LD BC,d16 */
            operandw = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            cpu->BC = operandw;
            break;
        case 0x08:  /* LD (a16),SP */
            operandw = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            mem_write(operandw, (uint8_t)(cpu->SP & 0xFF));
            mem_write(operandw+1, (uint8_t)((cpu->SP >> 8) & 0xFF));
            break;
        case 0x11:  /* LD DE,d16 */
            operandw = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            cpu->DE = operandw;
            break;
        case 0x21:  /* LD HL,d16 */
            operandw = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            cpu->HL = operandw;
            break;
        case 0x31:  /* LD SP,d16 */
            operandw = mem_read(cpu->PC+1) | (mem_read(cpu->PC+2) << 8);
            cpu->SP = operandw;
            break;
        case 0xC1:  /* POP BC */
            operandw |= (mem_read(cpu->SP++) << 8);
            operandw |= mem_read(cpu->SP++);
            cpu->BC = operandw;
            break;
        case 0xC5:  /* PUSH BC */
            mem_write(--cpu->SP, cpu->B);
            mem_write(--cpu->SP, cpu->C);
            break;
        case 0xD1:  /* POP DE */
            operandw |= (mem_read(cpu->SP++) << 8);
            operandw |= mem_read(cpu->SP++);
            cpu->DE = operandw;
            break;
        case 0xD5:  /* PUSH DE */
            mem_write(--cpu->SP, cpu->D);
            mem_write(--cpu->SP, cpu->E);
            break;
        case 0xE1:  /* POP HL */
            operandw |= (mem_read(cpu->SP++) << 8);
            operandw |= mem_read(cpu->SP++);
            cpu->HL = operandw;
            break;
        case 0xE5:  /* PUSH HL */
            mem_write(--cpu->SP, cpu->H);
            mem_write(--cpu->SP, cpu->L);
            break;
        case 0xF1:  /* POP AF */
            operandw |= (mem_read(cpu->SP++) << 8);
            operandw |= mem_read(cpu->SP++);
            cpu->AF = operandw;
            break;
        case 0xF5:  /* PUSH AF */
            mem_write(--cpu->SP, cpu->A);
            mem_write(--cpu->SP, cpu->F);
            break;
        case 0xF8:  /* LD HL,SP+r8 */
            cpu->HL = (cpu->SP + (int8_t)mem_read(cpu->PC+1));
            break;
        case 0xF9:  /* LD SP,HL */
            cpu->SP = cpu->HL;
            break;

        /* ------- 8-bit arithmetic/logic ------- */
        case 0x04:  /* INC B */
            ++cpu->B;
            break;
        case 0x05:  /* DEC B */
            --cpu->B;
            break;
        case 0x0C:  /* INC C */
            ++cpu->C;
            break;
        case 0x0D:  /* DEC C */
            --cpu->C;
            break;
        case 0x14:  /* INC D */
            ++cpu->D;
            break;
        case 0x15:  /* DEC D */
            --cpu->D;
            break;
        case 0x1C:  /* INC E */
            ++cpu->E;
            break;
        case 0x1D:  /* DEC E */
            --cpu->E;
            break;
        case 0x24:  /* INC H */
            ++cpu->H;
            break;
        case 0x25:  /* DEC H */
            --cpu->H;
            break;
        case 0x27:  /* DAA */
            break;
        case 0x2C:  /* INC L */
            ++cpu->L;
            break;
        case 0x2D:  /* DEC L */
            --cpu->L;
            break;
        case 0x2F:  /* CPL */
            cpu->A = ~cpu->A;
            break;
        case 0x34:  /* INC (HL) */
            mem_write(cpu->HL, mem_read(cpu->HL)+1);
            break;
        case 0x35:  /* DEC (HL) */
            mem_write(cpu->HL, mem_read(cpu->HL)-1);
            break;
        case 0x37:  /* SCF */
            cpu->flags.C = 0x1;
            break;
        case 0x3C:  /* INC A */
            ++cpu->A;
            break;
        case 0x3D:  /* DEC A */
            --cpu->A;
            break;
        case 0x3F:  /* CCF */
            cpu->flags.C = ~cpu->flags.C;
            break;
        case 0x80:  /* ADD A,B */
            cpu->A += cpu->B;
            cpu->flags.Z = (cpu->A == 0x0) ? 1 : 0;
            cpu->flags.N = (cpu->A == 0x0) ? 1 : 0;
            cpu->flags.H = (cpu->A == 0x0) ? 1 : 0;
            cpu->flags.C = (cpu->A == 0x0) ? 1 : 0;
            break;
        case 0x81:  /* ADD A,C */
            cpu->A += cpu->C;
            cpu->flags.Z = (cpu->A == 0x0) ? 1 : 0;
            cpu->flags.N = (cpu->A == 0x0) ? 1 : 0;
            cpu->flags.H = (cpu->A == 0x0) ? 1 : 0;
            cpu->flags.C = (cpu->A == 0x0) ? 1 : 0;
            break;
        case 0x82:  /* ADD A,D */
            cpu->A += cpu->D;
            break;
        case 0x83:  /* ADD A,E */
            cpu->A += cpu->E;
            break;
        case 0x84:  /* ADD A,H */
            cpu->A += cpu->H;
            break;
        case 0x85:  /* ADD A,L */
            cpu->A += cpu->L;
            break;
        case 0x86:  /* ADD A,(HL) */
            cpu->A += mem_read(cpu->HL);
            break;
        case 0x87:  /* ADD A,A */
            cpu->A += cpu->A;
            break;
        case 0x88:  /* ADC A,B */
            cpu->A += (cpu->B + cpu->flags.C);
            break;
        case 0x89:  /* ADC A,C */
            cpu->A += (cpu->C + cpu->flags.C);
            break;
        case 0x8A:  /* ADC A,D */
            cpu->A += (cpu->D + cpu->flags.C);
            break;
        case 0x8B:  /* ADC A,E */
            cpu->A += (cpu->E + cpu->flags.C);
            break;
        case 0x8C:  /* ADC A,H */
            cpu->A += (cpu->H + cpu->flags.C);
            break;
        case 0x8D:  /* ADC A,L */
            cpu->A += (cpu->L + cpu->flags.C);
            break;
        case 0x8E:  /* ADC A,(HL) */
            cpu->A += (mem_read(cpu->HL) + cpu->flags.C);
            break;
        case 0x8F:  /* ADC A,A */
            cpu->A += (cpu->A + cpu->flags.C);
            break;
        case 0x90:  /* SUB B */
            cpu->A -= cpu->B;
            break;
        case 0x91:  /* SUB C */
            cpu->A -= cpu->C;
            break;
        case 0x92:  /* SUB D */
            cpu->A -= cpu->D;
            break;
        case 0x93:  /* SUB E */
            cpu->A -= cpu->E;
            break;
        case 0x94:  /* SUB H */
            cpu->A -= cpu->H;
            break;
        case 0x95:  /* SUB L */
            cpu->A -= cpu->L;
            break;
        case 0x96:  /* SUB (HL) */
            cpu->A -= mem_read(cpu->HL);
            break;
        case 0x97:  /* SUB A */
            cpu->A -= cpu->A;
            break;
        case 0x98:  /* SBC A,B */
            cpu->A -= (cpu->B + cpu->flags.C);
            break;
        case 0x99:  /* SBC A,C */
            cpu->A -= (cpu->C + cpu->flags.C);
            break;
        case 0x9A:  /* SBC A,D */
            cpu->A -= (cpu->D + cpu->flags.C);
            break;
        case 0x9B:  /* SBC A,E */
            cpu->A -= (cpu->E + cpu->flags.C);
            break;
        case 0x9C:  /* SBC A,H */
            cpu->A -= (cpu->H + cpu->flags.C);
            break;
        case 0x9D:  /* SBC A,L */
            cpu->A -= (cpu->L + cpu->flags.C);
            break;
        case 0x9E:  /* SBC A,(HL) */
            cpu->A -= (mem_read(cpu->HL) + cpu->flags.C);
            break;
        case 0x9F:  /* SBC A,A */
            cpu->A -= (cpu->A + cpu->flags.C);
            break;
        case 0xA0:  /* AND B */
            cpu->A &= cpu->B;
            break;
        case 0xA1:  /* AND C */
            cpu->A &= cpu->C;
            break;
        case 0xA2:  /* AND D */
            cpu->A &= cpu->D;
            break;
        case 0xA3:  /* AND E */
            cpu->A &= cpu->E;
            break;
        case 0xA4:  /* AND H */
            cpu->A &= cpu->H;
            break;
        case 0xA5:  /* AND L */
            cpu->A &= cpu->L;
            break;
        case 0xA6:  /* AND (HL) */
            cpu->A &= mem_read(cpu->HL);
            break;
        case 0xA7:  /* AND A */
            cpu->A &= cpu->A;
            break;
        case 0xA8:  /* XOR B */
            cpu->A ^= cpu->B;
            break;
        case 0xA9:  /* XOR C */
            cpu->A ^= cpu->C;
            break;
        case 0xAA:  /* XOR D */
            cpu->A ^= cpu->D;
            break;
        case 0xAB:  /* XOR E */
            cpu->A ^= cpu->E;
            break;
        case 0xAC:  /* XOR H */
            cpu->A ^= cpu->H;
            break;
        case 0xAD:  /* XOR L */
            cpu->A ^= cpu->L;
            break;
        case 0xAE:  /* XOR (HL) */
            cpu->A ^= mem_read(cpu->HL);
            break;
        case 0xAF:  /* XOR A */
            cpu->A ^= cpu->A;
            break;
        case 0xB0:  /* OR B */
            cpu->A |= cpu->B;
            break;
        case 0xB1:  /* OR C */
            cpu->A |= cpu->C;
            break;
        case 0xB2:  /* OR D */
            cpu->A |= cpu->D;
            break;
        case 0xB3:  /* OR E */
            cpu->A |= cpu->E;
            break;
        case 0xB4:  /* OR H */
            cpu->A |= cpu->H;
            break;
        case 0xB5:  /* OR L */
            cpu->A |= cpu->L;
            break;
        case 0xB6:  /* OR (HL) */
            cpu->A |= mem_read(cpu->HL);
            break;
        case 0xB7:  /* OR A */
            cpu->A |= cpu->A;
            break;
        case 0xB8:  /* CP B */
            break;
        case 0xB9:  /* CP C */
            break;
        case 0xBA:  /* CP D */
            break;
        case 0xBB:  /* CP E */
            break;
        case 0xBC:  /* CP H */
            break;
        case 0xBD:  /* CP L */
            break;
        case 0xBE:  /* CP (HL) */
            cpu->A &= mem_read(cpu->HL);
            break;
        case 0xBF:  /* CP A */
            break;
        case 0xC6:  /* ADD A,d8 */
            break;
        case 0xCE:  /* ADC A,d8 */
            break;
        case 0xD6:  /* SUB d8 */
            break;
        case 0xDE:  /* SBC A,d8 */
            break;
        case 0xE6:  /* AND d8 */
            break;
        case 0xEE:  /* XOR d8 */
            break;
        case 0xF6:  /* OR d8 */
            break;
        case 0xFE:  /* CP d8 */
            break;

        /* ------- 16-bit arithmetic/logic ------- */
        case 0x03:  /* INC BC */
            ++cpu->BC;
            break;
        case 0x09:  /* ADD HL,BC */
            cpu->flags.N = 0x0;
            cpu->flags.H = ((((cpu->H & 0xF) + (cpu->B & 0xF)) & 0x10) == 0x10);
            cpu->flags.C = (((cpu->H  + cpu->B) & 0x100) == 0x100);
            cpu->HL += cpu->BC;
            break;
        case 0x0B:  /* DEC BC */
            --cpu->BC;
            break;
        case 0x13:  /* INC DE */
            ++cpu->DE;
            break;
        case 0x19:  /* ADD HL,DE */
            cpu->flags.N = 0x0;
            cpu->flags.H = ((((cpu->H & 0xF) + (cpu->D & 0xF)) & 0x10) == 0x10);
            cpu->flags.C = (((cpu->H  + cpu->D) & 0x100) == 0x100);
            cpu->HL += cpu->DE;
            break;
        case 0x1B:  /* DEC DE */
            --cpu->DE;
            break;
        case 0x23:  /* INC HL */
            ++cpu->HL;
            break;
        case 0x29:  /* ADD HL,HL */
            cpu->flags.N = 0x0;
            cpu->flags.H = ((((cpu->H & 0xF) + (cpu->H & 0xF)) & 0x10) == 0x10);
            cpu->flags.C = (((cpu->H  + cpu->H) & 0x100) == 0x100);
            cpu->HL += cpu->HL;
            break;
        case 0x2B:  /* DEC HL */
            --cpu->HL;
            break;
        case 0x33:  /* INC SP */
            ++cpu->SP;
            break;
        case 0x39:  /* ADD HL,SP */
            cpu->flags.N = 0x0;
            cpu->flags.H = ((((cpu->H & 0xF) + ((cpu->SP >> 8) & 0xF)) & 0x10) == 0x10);
            cpu->flags.C = (((cpu->H  + (cpu->SP >> 8)) & 0x100) == 0x100);
            cpu->HL += cpu->SP;
            break;
        case 0x3B:  /* DEC SP */
            --cpu->SP;
            break;
        case 0xE8:  /* ADD SP,r8 */
            cpu->SP += (int8_t)mem_read(cpu->PC+1);
            break;

        /* ------- 8-bit rotates/shifts ------- */
        case 0x07:  /* RLCA */
            break;
        case 0x0F:  /* RRCA */
            break;
        case 0x17:  /* RLA */
            break;
        case 0x1F:  /* RRA */
            break;
        case 0xCB:  /* PREFIX CB */
            {
                uint8_t cb_inst = mem_read(cpu->PC+1);
                uint8_t hl_val = 0x00;
                switch (cb_inst)
                {
                    case 0x00: /* RLC B */
                        break;
                    case 0x01: /* RLC C */
                        break;
                    case 0x02: /* RLC D */
                        break;
                    case 0x03: /* RLC E */
                        break;
                    case 0x04: /* RLC H */
                        break;
                    case 0x05: /* RLC L */
                        break;
                    case 0x06: /* RLC (HL) */
                        break;
                    case 0x07: /* RLC A */
                        break;
                    case 0x08: /* RRC B */
                        break;
                    case 0x09: /* RRC C */
                        break;
                    case 0x0A: /* RRC D */
                        break;
                    case 0x0B: /* RRC E */
                        break;
                    case 0x0C: /* RRC H */
                        break;
                    case 0x0D: /* RRC L */
                        break;
                    case 0x0E: /* RRC (HL) */
                        break;
                    case 0x0F: /* RRC A */
                        break;

                    case 0x10: /* RL B */
                        break;
                    case 0x11: /* RL C */
                        break;
                    case 0x12: /* RL D */
                        break;
                    case 0x13: /* RL E */
                        break;
                    case 0x14: /* RL H */
                        break;
                    case 0x15: /* RL L */
                        break;
                    case 0x16: /* RL (HL) */
                        break;
                    case 0x17: /* RL A */
                        break;
                    case 0x18: /* RR B */
                        break;
                    case 0x19: /* RR C */
                        break;
                    case 0x1A: /* RR D */
                        break;
                    case 0x1B: /* RR E */
                        break;
                    case 0x1C: /* RR H */
                        break;
                    case 0x1D: /* RR L */
                        break;
                    case 0x1E: /* RR (HL) */
                        break;
                    case 0x1F: /* RR A */
                        break;

                    case 0x20: /* SLA B */
                        cpu->flags.C = test_bit(cpu->B, 7);
                        cpu->B <<= 1;
                        break;
                    case 0x21: /* SLA C */
                        cpu->flags.C = test_bit(cpu->C, 7);
                        cpu->C <<= 1;
                        break;
                    case 0x22: /* SLA D */
                        cpu->flags.C = test_bit(cpu->D, 7);
                        cpu->D <<= 1;
                        break;
                    case 0x23: /* SLA E */
                        cpu->flags.C = test_bit(cpu->E, 7);
                        cpu->E <<= 1;
                        break;
                    case 0x24: /* SLA H */
                        cpu->flags.C = test_bit(cpu->H, 7);
                        cpu->H <<= 1;
                        break;
                    case 0x25: /* SLA L */
                        cpu->flags.C = test_bit(cpu->L, 7);
                        cpu->L <<= 1;
                        break;
                    case 0x26: /* SLA (HL) */
                        cpu->flags.C = test_bit(cpu->B, 7);
                        cpu->B <<= 1;
                        break;
                    case 0x27: /* SLA A */
                        cpu->flags.C = test_bit(cpu->A, 7);
                        cpu->A <<= 1;
                        break;
                    case 0x28: /* SRA B */
                        break;
                    case 0x29: /* SRA C */
                        break;
                    case 0x2A: /* SRA D */
                        break;
                    case 0x2B: /* SRA E */
                        break;
                    case 0x2C: /* SRA H */
                        break;
                    case 0x2D: /* SRA L */
                        break;
                    case 0x2E: /* SRA (HL) */
                        break;
                    case 0x2F: /* SRA A */
                        break;

                    case 0x30: /* SWAP B */
                        temp_nibble = (cpu->B & HI_NIBBLE) >> 4;
                        cpu->B <<= 4;
                        cpu->B |= temp_nibble;
                        break;
                    case 0x31: /* SWAP C */
                        temp_nibble = (cpu->C & HI_NIBBLE) >> 4;
                        cpu->C <<= 4;
                        cpu->C |= temp_nibble;
                        break;
                    case 0x32: /* SWAP D */
                        temp_nibble = (cpu->D & HI_NIBBLE) >> 4;
                        cpu->D <<= 4;
                        cpu->D |= temp_nibble;
                        break;
                    case 0x33: /* SWAP E */
                        temp_nibble = (cpu->E & HI_NIBBLE) >> 4;
                        cpu->E <<= 4;
                        cpu->E |= temp_nibble;
                        break;
                    case 0x34: /* SWAP H */
                        temp_nibble = (cpu->H & HI_NIBBLE) >> 4;
                        cpu->H <<= 4;
                        cpu->H |= temp_nibble;
                        break;
                    case 0x35: /* SWAP L */
                        temp_nibble = (cpu->L & HI_NIBBLE) >> 4;
                        cpu->L <<= 4;
                        cpu->L |= temp_nibble;
                        break;
                    case 0x36: /* SWAP (HL) */
                        hl_val = mem_read(cpu->HL);
                        temp_nibble = (hl_val & HI_NIBBLE) >> 4;
                        hl_val <<= 4;
                        hl_val |= temp_nibble;
                        mem_write(cpu->HL, hl_val);
                        break;
                    case 0x37: /* SWAP A */
                        temp_nibble = (cpu->A & HI_NIBBLE) >> 4;
                        cpu->A <<= 4;
                        cpu->A |= temp_nibble;
                        break;
                    case 0x38: /* SRL B */
                        break;
                    case 0x39: /* SRL C */
                        break;
                    case 0x3A: /* SRL D */
                        break;
                    case 0x3B: /* SRL E */
                        break;
                    case 0x3C: /* SRL H */
                        break;
                    case 0x3D: /* SRL L */
                        break;
                    case 0x3E: /* SRL (HL) */
                        break;
                    case 0x3F: /* SRL A */
                        break;

                    case 0x40: /* BIT 0,B */
                        cpu->flags.Z = !test_bit(cpu->B, 0);
                        cpu->flags.N = 0;
                        cpu->flags.H = 1;
                        break;
                    case 0x41: /* BIT 0,C */
                        cpu->flags.Z = !test_bit(cpu->C, 0);
                        cpu->flags.N = 0;
                        cpu->flags.H = 1;
                        break;
                    case 0x42: /* BIT 0,D */
                        cpu->flags.Z = !test_bit(cpu->D, 0);
                        cpu->flags.N = 0;
                        cpu->flags.H = 1;
                        break;
                    case 0x43: /* BIT 0,E */
                        cpu->flags.Z = !test_bit(cpu->E, 0);
                        cpu->flags.N = 0;
                        cpu->flags.H = 1;
                        break;
                    case 0x44: /* BIT 0,H */
                        cpu->flags.Z = !test_bit(cpu->H, 0);
                        cpu->flags.N = 0;
                        cpu->flags.H = 1;
                        break;
                    case 0x45: /* BIT 0,L */
                        cpu->flags.Z = !test_bit(cpu->L, 0);
                        cpu->flags.N = 0;
                        cpu->flags.H = 1;
                        break;
                    case 0x46: /* BIT 0,(HL) */
                        cpu->flags.Z = !test_bit(mem_read(cpu->HL), 0);
                        cpu->flags.N = 0;
                        cpu->flags.H = 1;
                        break;
                    case 0x47: /* BIT 0,A */
                        test_bit(cpu->A, 0);
                        cpu->flags.N = 0;
                        cpu->flags.H = 1;
                        break;
                    case 0x48: /* BIT 1,B */
                        break;
                    case 0x49: /* BIT 1,C */
                        break;
                    case 0x4A: /* BIT 1,D */
                        break;
                    case 0x4B: /* BIT 1,E */
                        break;
                    case 0x4C: /* BIT 1,H */
                        break;
                    case 0x4D: /* BIT 1,L */
                        break;
                    case 0x4E: /* BIT 1,(HL) */
                        break;
                    case 0x4F: /* BIT 1,A */
                        break;

                    case 0x50: /* BIT 2,B */
                        break;
                    case 0x51: /* BIT 2,C */
                        break;
                    case 0x52: /* BIT 2,D */
                        break;
                    case 0x53: /* BIT 2,E */
                        break;
                    case 0x54: /* BIT 2,H */
                        break;
                    case 0x55: /* BIT 2,L */
                        break;
                    case 0x56: /* BIT 2,(HL) */
                        break;
                    case 0x57: /* BIT 2,A */
                        break;
                    case 0x58: /* BIT 3,B */
                        break;
                    case 0x59: /* BIT 3,C */
                        break;
                    case 0x5A: /* BIT 3,D */
                        break;
                    case 0x5B: /* BIT 3,E */
                        break;
                    case 0x5C: /* BIT 3,H */
                        break;
                    case 0x5D: /* BIT 3,L */
                        break;
                    case 0x5E: /* BIT 3,(HL) */
                        break;
                    case 0x5F: /* BIT 3,A */
                        break;

                    case 0x60: /* BIT 4,B */
                        break;
                    case 0x61: /* BIT 4,C */
                        break;
                    case 0x62: /* BIT 4,D */
                        break;
                    case 0x63: /* BIT 4,E */
                        break;
                    case 0x64: /* BIT 4,H */
                        break;
                    case 0x65: /* BIT 4,L */
                        break;
                    case 0x66: /* BIT 4,(HL) */
                        break;
                    case 0x67: /* BIT 4,A */
                        break;
                    case 0x68: /* BIT 5,B */
                        break;
                    case 0x69: /* BIT 5,C */
                        break;
                    case 0x6A: /* BIT 5,D */
                        break;
                    case 0x6B: /* BIT 5,E */
                        break;
                    case 0x6C: /* BIT 5,H */
                        break;
                    case 0x6D: /* BIT 5,L */
                        break;
                    case 0x6E: /* BIT 5,(HL) */
                        break;
                    case 0x6F: /* BIT 5,A */
                        break;

                    case 0x70: /* BIT 6,B */
                        break;
                    case 0x71: /* BIT 6,C */
                        break;
                    case 0x72: /* BIT 6,D */
                        break;
                    case 0x73: /* BIT 6,E */
                        break;
                    case 0x74: /* BIT 6,H */
                        break;
                    case 0x75: /* BIT 6,L */
                        break;
                    case 0x76: /* BIT 6,(HL) */
                        break;
                    case 0x77: /* BIT 6,A */
                        break;
                    case 0x78: /* BIT 7,B */
                        break;
                    case 0x79: /* BIT 7,C */
                        break;
                    case 0x7A: /* BIT 7,D */
                        break;
                    case 0x7B: /* BIT 7,E */
                        break;
                    case 0x7C: /* BIT 7,H */
                        break;
                    case 0x7D: /* BIT 7,L */
                        break;
                    case 0x7E: /* BIT 7,(HL) */
                        break;
                    case 0x7F: /* BIT 7,A */
                        break;

                    case 0x80: /* RES 0,B */
                        break;
                    case 0x81: /* RES 0,C */
                        break;
                    case 0x82: /* RES 0,D */
                        break;
                    case 0x83: /* RES 0,E */
                        break;
                    case 0x84: /* RES 0,H */
                        break;
                    case 0x85: /* RES 0,L */
                        break;
                    case 0x86: /* RES 0,(HL) */
                        break;
                    case 0x87: /* RES 0,A */
                        break;
                    case 0x88: /* RES 1,B */
                        break;
                    case 0x89: /* RES 1,C */
                        break;
                    case 0x8A: /* RES 1,D */
                        break;
                    case 0x8B: /* RES 1,E */
                        break;
                    case 0x8C: /* RES 1,H */
                        break;
                    case 0x8D: /* RES 1,L */
                        break;
                    case 0x8E: /* RES 1,(HL) */
                        break;
                    case 0x8F: /* RES 1,A */
                        break;

                    case 0x90: /* RES 2,B */
                        break;
                    case 0x91: /* RES 2,C */
                        break;
                    case 0x92: /* RES 2,D */
                        break;
                    case 0x93: /* RES 2,E */
                        break;
                    case 0x94: /* RES 2,H */
                        break;
                    case 0x95: /* RES 2,L */
                        break;
                    case 0x96: /* RES 2,(HL) */
                        break;
                    case 0x97: /* RES 2,A */
                        break;
                    case 0x98: /* RES 3,B */
                        break;
                    case 0x99: /* RES 3,C */
                        break;
                    case 0x9A: /* RES 3,D */
                        break;
                    case 0x9B: /* RES 3,E */
                        break;
                    case 0x9C: /* RES 3,H */
                        break;
                    case 0x9D: /* RES 3,L */
                        break;
                    case 0x9E: /* RES 3,(HL) */
                        break;
                    case 0x9F: /* RES 3,A */
                        break;

                    case 0xA0: /* RES 4,B */
                        break;
                    case 0xA1: /* RES 4,C */
                        break;
                    case 0xA2: /* RES 4,D */
                        break;
                    case 0xA3: /* RES 4,E */
                        break;
                    case 0xA4: /* RES 4,H */
                        break;
                    case 0xA5: /* RES 4,L */
                        break;
                    case 0xA6: /* RES 4,(HL) */
                        break;
                    case 0xA7: /* RES 4,A */
                        break;
                    case 0xA8: /* RES 5,B */
                        break;
                    case 0xA9: /* RES 5,C */
                        break;
                    case 0xAA: /* RES 5,D */
                        break;
                    case 0xAB: /* RES 5,E */
                        break;
                    case 0xAC: /* RES 5,H */
                        break;
                    case 0xAD: /* RES 5,L */
                        break;
                    case 0xAE: /* RES 5,(HL) */
                        break;
                    case 0xAF: /* RES 5,A */
                        break;

                    case 0xB0: /* RES 6,B */
                        break;
                    case 0xB1: /* RES 6,C */
                        break;
                    case 0xB2: /* RES 6,D */
                        break;
                    case 0xB3: /* RES 6,E */
                        break;
                    case 0xB4: /* RES 6,H */
                        break;
                    case 0xB5: /* RES 6,L */
                        break;
                    case 0xB6: /* RES 6,(HL) */
                        break;
                    case 0xB7: /* RES 6,A */
                        break;
                    case 0xB8: /* RES 7,B */
                        break;
                    case 0xB9: /* RES 7,C */
                        break;
                    case 0xBA: /* RES 7,D */
                        break;
                    case 0xBB: /* RES 7,E */
                        break;
                    case 0xBC: /* RES 7,H */
                        break;
                    case 0xBD: /* RES 7,L */
                        break;
                    case 0xBE: /* RES 7,(HL) */
                        break;
                    case 0xBF: /* RES 7,A */
                        break;

                    case 0xC0: /* SET 0,B */
                        break;
                    case 0xC1: /* SET 0,C */
                        break;
                    case 0xC2: /* SET 0,D */
                        break;
                    case 0xC3: /* SET 0,E */
                        break;
                    case 0xC4: /* SET 0,H */
                        break;
                    case 0xC5: /* SET 0,L */
                        break;
                    case 0xC6: /* SET 0,(HL) */
                        break;
                    case 0xC7: /* SET 0,A */
                        break;
                    case 0xC8: /* SET 1,B */
                        break;
                    case 0xC9: /* SET 1,C */
                        break;
                    case 0xCA: /* SET 1,D */
                        break;
                    case 0xCB: /* SET 1,E */
                        break;
                    case 0xCC: /* SET 1,H */
                        break;
                    case 0xCD: /* SET 1,L */
                        break;
                    case 0xCE: /* SET 1,(HL) */
                        break;
                    case 0xCF: /* SET 1,A */
                        break;

                    case 0xD0: /* SET 2,B */
                        break;
                    case 0xD1: /* SET 2,C */
                        break;
                    case 0xD2: /* SET 2,D */
                        break;
                    case 0xD3: /* SET 2,E */
                        break;
                    case 0xD4: /* SET 2,H */
                        break;
                    case 0xD5: /* SET 2,L */
                        break;
                    case 0xD6: /* SET 2,(HL) */
                        break;
                    case 0xD7: /* SET 2,A */
                        break;
                    case 0xD8: /* SET 3,B */
                        break;
                    case 0xD9: /* SET 3,C */
                        break;
                    case 0xDA: /* SET 3,D */
                        break;
                    case 0xDB: /* SET 3,E */
                        break;
                    case 0xDC: /* SET 3,H */
                        break;
                    case 0xDD: /* SET 3,L */
                        break;
                    case 0xDE: /* SET 3,(HL) */
                        break;
                    case 0xDF: /* SET 3,A */
                        break;

                    case 0xE0: /* SET 4,B */
                        break;
                    case 0xE1: /* SET 4,C */
                        break;
                    case 0xE2: /* SET 4,D */
                        break;
                    case 0xE3: /* SET 4,E */
                        break;
                    case 0xE4: /* SET 4,H */
                        break;
                    case 0xE5: /* SET 4,L */
                        break;
                    case 0xE6: /* SET 4,(HL) */
                        break;
                    case 0xE7: /* SET 4,A */
                        break;
                    case 0xE8: /* SET 5,B */
                        break;
                    case 0xE9: /* SET 5,C */
                        break;
                    case 0xEA: /* SET 5,D */
                        break;
                    case 0xEB: /* SET 5,E */
                        break;
                    case 0xEC: /* SET 5,H */
                        break;
                    case 0xED: /* SET 5,L */
                        break;
                    case 0xEE: /* SET 5,(HL) */
                        break;
                    case 0xEF: /* SET 5,A */
                        break;

                    case 0xF0: /* SET 6,B */
                        break;
                    case 0xF1: /* SET 6,C */
                        break;
                    case 0xF2: /* SET 6,D */
                        break;
                    case 0xF3: /* SET 6,E */
                        break;
                    case 0xF4: /* SET 6,H */
                        break;
                    case 0xF5: /* SET 6,L */
                        break;
                    case 0xF6: /* SET 6,(HL) */
                        break;
                    case 0xF7: /* SET 6,A */
                        break;
                    case 0xF8: /* SET 7,B */
                        break;
                    case 0xF9: /* SET 7,C */
                        break;
                    case 0xFA: /* SET 7,D */
                        break;
                    case 0xFB: /* SET 7,E */
                        break;
                    case 0xFC: /* SET 7,H */
                        break;
                    case 0xFD: /* SET 7,L */
                        break;
                    case 0xFE: /* SET 7,(HL) */
                        break;
                    case 0xFF: /* SET 7,A */
                        break;
                }
            }

        /* Unimplemented opcodes */
        /* All of these crash the system */
        case 0xD3:
        case 0xDB:
        case 0xDD:
        case 0xE3:
        case 0xE4:
        case 0xEB:
        case 0xEC:
        case 0xED:
        case 0xF4:
        case 0xFC:
        case 0xFD:
            illegal();
            break;

        default:
            sprintf(stderr, "Unrecognized opcode!");
            exit(1); /* Exited with error */
            break;
    }
}
