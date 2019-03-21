/* cpu.c
 * Implements the CPU
 */

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "mem.h"

static void illegal(void);

void write_hi_byte(uint8_t byte, uint16_t reg)
{
    reg &= (byte << 8);
}

void run(CPU* cpu)
{
    while(1)
    {
        switch(memory[cpu->PC])
        {
            case 0x00:  /* NOP */
            case 0x01:  /* LD BC,d16 */
            case 0x02:  /* LD (BC),A */
            case 0x03:  /* INC BC */
            case 0x04:  /* INC B */
            case 0x05:  /* DEC B */
            case 0x06:  /* LD B,d8 */
            case 0x07:  /* RLCA */
            case 0x08:  /* LD (a16),SP */
            case 0x09:  /* ADD HL,BC */
            case 0x0A:  /* LD A,(BC) */
            case 0x0B:  /* DEC BC */
            case 0x0C:  /* INC C */
            case 0x0D:  /* DEC C */
            case 0x0E:  /* LD C,d8 */
            case 0x0F:  /* RRCA */

            case 0x10:  /* STOP 0 */
            case 0x11:  /* LD DE,d16 */
            case 0x12:  /* INC DE */
            case 0x14:  /* INC D */
            case 0x15:  /* DEC D */
            case 0x16:  /* LD D,d8 */
            case 0x17:  /* RLA */
            case 0x18:  /* JR r8 */
            case 0x19:  /* ADD HL,DE */
            case 0x1A:  /* LD A,(DE) */
            case 0x1B:  /* DEC DE */
            case 0x1C:  /* INC E */
            case 0x1D:  /* DEC E */
            case 0x1E:  /* LD E,d8 */
            case 0x1F:  /* RRA */

            case 0x20:  /* JR NZ,r8 */
            case 0x21:  /* LD HL,d16 */
            case 0x22:  /* LD (HL+),A */
            case 0x23:  /* INC HL */
            case 0x24:  /* INC H */
            case 0x25:  /* DEC H */
            case 0x26:  /* LD H,d8 */
            case 0x27:  /* DAA */
            case 0x28:  /* JR Z,r8 */
            case 0x29:  /* ADD HL,HL */
            case 0x2A:  /* LD A,(HL+) */
            case 0x2B:  /* DEC HL */
            case 0x2C:  /* INC L */
            case 0x2D:  /* DEC L */
            case 0x2E:  /* LD L,d8 */
            case 0x2F:  /* CPL */

            case 0x30:  /* JR NC,r8 */
            case 0x31:  /* LD SP,d16 */
            case 0x32:  /* LD (HL-),A */
            case 0x33:  /* INC SP */
            case 0x34:  /* INC (HL) */
            case 0x35:  /* DEC (HL) */
            case 0x36:  /* LD (HL),d8 */
            case 0x37:  /* SCF */
            case 0x38:  /* JR C,r8 */
            case 0x39:  /* ADD HL,SP */
            case 0x3A:  /* LD A,(HL-) */
            case 0x3B:  /* DEC SP */
            case 0x3C:  /* INC A */
            case 0x3D:  /* DEC A */
            case 0x3E:  /* LD A,d8 */
            case 0x3F:  /* CCF */

            case 0x40:  /* LD B,B */
            case 0x41:  /* LD B,C */
            case 0x42:  /* LD B,D */
            case 0x43:  /* LD B,E */
            case 0x44:  /* LD B,H */
            case 0x45:  /* LD B,L */
            case 0x46:  /* LD B,(HL) */
            case 0x47:  /* LD B,A */
            case 0x48:  /* LD C,B */
            case 0x49:  /* LD C,C */
            case 0x4A:  /* LD C,D */
            case 0x4B:  /* LD C,E */
            case 0x4C:  /* LD C,H */
            case 0x4D:  /* LD C,L */
            case 0x4E:  /* LD C,(HL) */
            case 0x4F:  /* LD C,A */

            case 0x50:  /* LD D,B */
            case 0x51:  /* LD D,C */
            case 0x52:  /* LD D,D */
            case 0x53:  /* LD D,E */
            case 0x54:  /* LD D,H */
            case 0x55:  /* LD D,L */
            case 0x56:  /* LD D,(HL) */
            case 0x57:  /* LD D,A */
            case 0x58:  /* LD E,B */
            case 0x59:  /* LD E,C */
            case 0x5A:  /* LD E,D */
            case 0x5B:  /* LD E,E */
            case 0x5C:  /* LD E,H */
            case 0x5D:  /* LD E,L */
            case 0x5E:  /* LD E,(HL) */
            case 0x5F:  /* LD E,A */

            case 0x60:  /* LD H,B */
            case 0x61:  /* LD H,C */
            case 0x62:  /* LD H,D */
            case 0x63:  /* LD H,E */
            case 0x64:  /* LD H,H */
            case 0x65:  /* LD H,L */
            case 0x66:  /* LD H,(HL) */
            case 0x67:  /* LD H,A */
            case 0x68:  /* LD L,B */
            case 0x69:  /* LD L,C */
            case 0x6A:  /* LD L,D */
            case 0x6B:  /* LD L,E */
            case 0x6C:  /* LD L,H */
            case 0x6D:  /* LD L,L */
            case 0x6E:  /* LD L,(HL) */
            case 0x6F:  /* LD L,A */

            case 0x70:  /* LD (HL),B */
            case 0x71:  /* LD (HL),C */
            case 0x72:  /* LD (HL),D */
            case 0x73:  /* LD (HL),E */
            case 0x74:  /* LD (HL),H */
            case 0x75:  /* LD (HL),L */
            case 0x76:  /* HALT */
            case 0x77:  /* LD (HL),A */
            case 0x78:  /* LD A,B */
            case 0x79:  /* LD A,C */
            case 0x7A:  /* LD A,D */
            case 0x7B:  /* LD A,E */
            case 0x7C:  /* LD A,H */
            case 0x7D:  /* LD A,L */
            case 0x7E:  /* LD A,(HL) */
            case 0x7F:  /* LD A,A */

            case 0x80:  /* ADD A,B */
            case 0x81:  /* ADD A,C */
            case 0x82:  /* ADD A,D */
            case 0x83:  /* ADD A,E */
            case 0x84:  /* ADD A,H */
            case 0x85:  /* ADD A,L */
            case 0x86:  /* ADD A,(HL) */
            case 0x87:  /* ADD A,B */
            case 0x88:  /* ADC A,B */
            case 0x89:  /* ADC A,C */
            case 0x8A:  /* ADC A,D */
            case 0x8B:  /* ADC A,E */
            case 0x8C:  /* ADC A,H */
            case 0x8D:  /* ADC A,L */
            case 0x8E:  /* ADC A,(HL) */
            case 0x8F:  /* ADC A,A */

            case 0x90:  /* SUB B */
            case 0x91:  /* SUB C */
            case 0x92:  /* SUB D */
            case 0x93:  /* SUB E */
            case 0x94:  /* SUB H */
            case 0x95:  /* SUB L */
            case 0x96:  /* SUB (HL) */
            case 0x97:  /* SUB A */
            case 0x98:  /* SBC A,B */
            case 0x99:  /* SBC A,C */
            case 0x9A:  /* SBC A,D */
            case 0x9B:  /* SBC A,E */
            case 0x9C:  /* SBC A,H */
            case 0x9D:  /* SBC A,L */
            case 0x9E:  /* SBC A,(HL) */
            case 0x9F:  /* SBC A,A */

            case 0xA0:  /* AND B */
            case 0xA1:  /* AND C */
            case 0xA2:  /* AND D */
            case 0xA3:  /* AND E */
            case 0xA4:  /* AND H */
            case 0xA5:  /* AND L */
            case 0xA6:  /* AND (HL) */
            case 0xA7:  /* AND A */
            case 0xA8:  /* XOR B */
            case 0xA9:  /* XOR C */
            case 0xAA:  /* XOR D */
            case 0xAB:  /* XOR E */
            case 0xAC:  /* XOR H */
            case 0xAD:  /* XOR L */
            case 0xAE:  /* XOR (HL) */
            case 0xAF:  /* XOR A */

            case 0xB0:  /* OR B */
            case 0xB1:  /* OR C */
            case 0xB2:  /* OR D */
            case 0xB3:  /* OR E */
            case 0xB4:  /* OR H */
            case 0xB5:  /* OR L */
            case 0xB6:  /* OR (HL) */
            case 0xB7:  /* OR A */
            case 0xB8:  /* CP B */
            case 0xB9:  /* CP C */
            case 0xBA:  /* CP D */
            case 0xBB:  /* CP E */
            case 0xBC:  /* CP H */
            case 0xBD:  /* CP L */
            case 0xBE:  /* CP (HL) */
            case 0xBF:  /* CP A */

            case 0xC0:  /* RET NZ */
            case 0xC1:  /* POP BC */
            case 0xC2:  /* JP NZ,a16 */
            case 0xC3:  /* JP a16 */
            case 0xC4:  /* CALL NZ,a16 */
            case 0xC5:  /* PUSH BC */
            case 0xC6:  /* ADD A,d8 */
            case 0xC7:  /* RST 00H */
            case 0xC8:  /* RET Z */
            case 0xC9:  /* RET */
            case 0xCA:  /* JP Z,a16 */
            case 0xCC:  /* CALL Z,a16 */
            case 0xCD:  /* CALL a16 */
            case 0xCE:  /* ADC A,d8 */
            case 0xCF:  /* RST 08H */

            case 0xD0:  /* RET NC */
            case 0xD1:  /* POP DE */
            case 0xD2:  /* JP NC,a16 */
            case 0xD4:  /* CALL NC,a16 */
            case 0xD5:  /* PUSH DE */
            case 0xD6:  /* SUB d8 */
            case 0xD7:  /* RST 10H */
            case 0xD8:  /* RET C */
            case 0xD9:  /* RETI */
            case 0xDA:  /* JP C,a16 */
            case 0xDC:  /* CALL C,a16 */
            case 0xDE:  /* SBC A,d8 */
            case 0xDF:  /* RST 18H */

            case 0xE0:  /* LDH (a8),A */
            case 0xE1:  /* POP HL */
            case 0xE2:  /* LD (C),A */
            case 0xE5:  /* PUSH HL */
            case 0xE6:  /* AND d8 */
            case 0xE7:  /* RST 20H */
            case 0xE8:  /* ADD SP,r8 */
            case 0xE9:  /* JP (HL) */
            case 0xEA:  /* LD (a16),A */
            case 0xEE:  /* XOR d8 */
            case 0xEF:  /* RST 28H */

            case 0xF0:  /* LDH A,(a8) */
            case 0xF1:  /* POP AF */
            case 0xF2:  /* LD A,(C) */
            case 0xF3:  /* DI */
            case 0xF5:  /* PUSH AF */
            case 0xF6:  /* OR d8 */
            case 0xF7:  /* RST 30H */
            case 0xF8:  /* LD HL,SP+r8 */
            case 0xF9:  /* LD SP,HL */
            case 0xFA:  /* LD A,(a16) */
            case 0xFB:  /* EI */
            case 0xFE:  /* CP */
            case 0xFF:  /* RST 38H */

                /* Miscellaneous/control */

                /* Jumps/calls */

                /* 8-bit loads/stores */

                /* 16-bit loads/stores */

                /* 8-bit arithmetic/logic */

                /* 16-bit arithmetic/logic */

                /* 8-bit rotates/shifts */
            case 0xCB:  /* PREFIX CB */
                {
                    switch (memory[cpu->PC+1])
                    {
                        case 0x00: /* RLC B */
                        case 0x01: /* RLC C */
                        case 0x02: /* RLC D */
                        case 0x03: /* RLC E */
                        case 0x04: /* RLC H */
                        case 0x05: /* RLC L */
                        case 0x06: /* RLC (HL) */
                        case 0x07: /* RLC A */
                        case 0x08: /* RRC B */
                        case 0x09: /* RRC C */
                        case 0x0A: /* RRC D */
                        case 0x0B: /* RRC E */
                        case 0x0C: /* RRC H */
                        case 0x0D: /* RRC L */
                        case 0x0E: /* RRC (HL) */
                        case 0x0F: /* RRC A */

                        case 0x10: /* RL B */
                        case 0x11: /* RL C */
                        case 0x12: /* RL D */
                        case 0x13: /* RL E */
                        case 0x14: /* RL H */
                        case 0x15: /* RL L */
                        case 0x16: /* RL (HL) */
                        case 0x17: /* RL A */
                        case 0x18: /* RR B */
                        case 0x19: /* RR C */
                        case 0x1A: /* RR D */
                        case 0x1B: /* RR E */
                        case 0x1C: /* RR H */
                        case 0x1D: /* RR L */
                        case 0x1E: /* RR (HL) */
                        case 0x1F: /* RR A */

                        case 0x20: /* SLA B */
                        case 0x21: /* SLA C */
                        case 0x22: /* SLA D */
                        case 0x23: /* SLA E */
                        case 0x24: /* SLA H */
                        case 0x25: /* SLA L */
                        case 0x26: /* SLA (HL) */
                        case 0x27: /* SLA A */
                        case 0x28: /* SRA B */
                        case 0x29: /* SRA C */
                        case 0x2A: /* SRA D */
                        case 0x2B: /* SRA E */
                        case 0x2C: /* SRA H */
                        case 0x2D: /* SRA L */
                        case 0x2E: /* SRA (HL) */
                        case 0x2F: /* SRA A */

                        case 0x30: /* SWAP B */
                        case 0x31: /* SWAP C */
                        case 0x32: /* SWAP D */
                        case 0x33: /* SWAP E */
                        case 0x34: /* SWAP H */
                        case 0x35: /* SWAP L */
                        case 0x36: /* SWAP (HL) */
                        case 0x37: /* SWAP A */
                        case 0x38: /* SRL B */
                        case 0x39: /* SRL C */
                        case 0x3A: /* SRL D */
                        case 0x3B: /* SRL E */
                        case 0x3C: /* SRL H */
                        case 0x3D: /* SRL L */
                        case 0x3E: /* SRL (HL) */
                        case 0x3F: /* SRL A */

                        case 0x40: /* BIT 0,B */
                        case 0x41: /* BIT 0,C */
                        case 0x42: /* BIT 0,D */
                        case 0x43: /* BIT 0,E */
                        case 0x44: /* BIT 0,H */
                        case 0x45: /* BIT 0,L */
                        case 0x46: /* BIT 0,(HL) */
                        case 0x47: /* BIT 0,A */
                        case 0x48: /* BIT 1,B */
                        case 0x49: /* BIT 1,C */
                        case 0x4A: /* BIT 1,D */
                        case 0x4B: /* BIT 1,E */
                        case 0x4C: /* BIT 1,H */
                        case 0x4D: /* BIT 1,L */
                        case 0x4E: /* BIT 1,(HL) */
                        case 0x4F: /* BIT 1,A */

                        case 0x50: /* BIT 2,B */
                        case 0x51: /* BIT 2,C */
                        case 0x52: /* BIT 2,D */
                        case 0x53: /* BIT 2,E */
                        case 0x54: /* BIT 2,H */
                        case 0x55: /* BIT 2,L */
                        case 0x56: /* BIT 2,(HL) */
                        case 0x57: /* BIT 2,A */
                        case 0x58: /* BIT 3,B */
                        case 0x59: /* BIT 3,C */
                        case 0x5A: /* BIT 3,D */
                        case 0x5B: /* BIT 3,E */
                        case 0x5C: /* BIT 3,H */
                        case 0x5D: /* BIT 3,L */
                        case 0x5E: /* BIT 3,(HL) */
                        case 0x5F: /* BIT 3,A */

                        case 0x60: /* BIT 4,B */
                        case 0x61: /* BIT 4,C */
                        case 0x62: /* BIT 4,D */
                        case 0x63: /* BIT 4,E */
                        case 0x64: /* BIT 4,H */
                        case 0x65: /* BIT 4,L */
                        case 0x66: /* BIT 4,(HL) */
                        case 0x67: /* BIT 4,A */
                        case 0x68: /* BIT 5,B */
                        case 0x69: /* BIT 5,C */
                        case 0x6A: /* BIT 5,D */
                        case 0x6B: /* BIT 5,E */
                        case 0x6C: /* BIT 5,H */
                        case 0x6D: /* BIT 5,L */
                        case 0x6E: /* BIT 5,(HL) */
                        case 0x6F: /* BIT 5,A */

                        case 0x70: /* BIT 6,B */
                        case 0x71: /* BIT 6,C */
                        case 0x72: /* BIT 6,D */
                        case 0x73: /* BIT 6,E */
                        case 0x74: /* BIT 6,H */
                        case 0x75: /* BIT 6,L */
                        case 0x76: /* BIT 6,(HL) */
                        case 0x77: /* BIT 6,A */
                        case 0x78: /* BIT 7,B */
                        case 0x79: /* BIT 7,C */
                        case 0x7A: /* BIT 7,D */
                        case 0x7B: /* BIT 7,E */
                        case 0x7C: /* BIT 7,H */
                        case 0x7D: /* BIT 7,L */
                        case 0x7E: /* BIT 7,(HL) */
                        case 0x7F: /* BIT 7,A */

                        case 0x80: /* RES 0,B */
                        case 0x81: /* RES 0,C */
                        case 0x82: /* RES 0,D */
                        case 0x83: /* RES 0,E */
                        case 0x84: /* RES 0,H */
                        case 0x85: /* RES 0,L */
                        case 0x86: /* RES 0,(HL) */
                        case 0x87: /* RES 0,A */
                        case 0x88: /* RES 1,B */
                        case 0x89: /* RES 1,C */
                        case 0x8A: /* RES 1,D */
                        case 0x8B: /* RES 1,E */
                        case 0x8C: /* RES 1,H */
                        case 0x8D: /* RES 1,L */
                        case 0x8E: /* RES 1,(HL) */
                        case 0x8F: /* RES 1,A */

                        case 0x90: /* RES 2,B */
                        case 0x91: /* RES 2,C */
                        case 0x92: /* RES 2,D */
                        case 0x93: /* RES 2,E */
                        case 0x94: /* RES 2,H */
                        case 0x95: /* RES 2,L */
                        case 0x96: /* RES 2,(HL) */
                        case 0x97: /* RES 2,A */
                        case 0x98: /* RES 3,B */
                        case 0x99: /* RES 3,C */
                        case 0x9A: /* RES 3,D */
                        case 0x9B: /* RES 3,E */
                        case 0x9C: /* RES 3,H */
                        case 0x9D: /* RES 3,L */
                        case 0x9E: /* RES 3,(HL) */
                        case 0x9F: /* RES 3,A */

                        case 0xA0: /* RES 4,B */
                        case 0xA1: /* RES 4,C */
                        case 0xA2: /* RES 4,D */
                        case 0xA3: /* RES 4,E */
                        case 0xA4: /* RES 4,H */
                        case 0xA5: /* RES 4,L */
                        case 0xA6: /* RES 4,(HL) */
                        case 0xA7: /* RES 4,A */
                        case 0xA8: /* RES 5,B */
                        case 0xA9: /* RES 5,C */
                        case 0xAA: /* RES 5,D */
                        case 0xAB: /* RES 5,E */
                        case 0xAC: /* RES 5,H */
                        case 0xAD: /* RES 5,L */
                        case 0xAE: /* RES 5,(HL) */
                        case 0xAF: /* RES 5,A */

                        case 0xB0: /* RES 6,B */
                        case 0xB1: /* RES 6,C */
                        case 0xB2: /* RES 6,D */
                        case 0xB3: /* RES 6,E */
                        case 0xB4: /* RES 6,H */
                        case 0xB5: /* RES 6,L */
                        case 0xB6: /* RES 6,(HL) */
                        case 0xB7: /* RES 6,A */
                        case 0xB8: /* RES 7,B */
                        case 0xB9: /* RES 7,C */
                        case 0xBA: /* RES 7,D */
                        case 0xBB: /* RES 7,E */
                        case 0xBC: /* RES 7,H */
                        case 0xBD: /* RES 7,L */
                        case 0xBE: /* RES 7,(HL) */
                        case 0xBF: /* RES 7,A */

                        case 0xC0: /* SET 0,B */
                        case 0xC1: /* SET 0,C */
                        case 0xC2: /* SET 0,D */
                        case 0xC3: /* SET 0,E */
                        case 0xC4: /* SET 0,H */
                        case 0xC5: /* SET 0,L */
                        case 0xC6: /* SET 0,(HL) */
                        case 0xC7: /* SET 0,A */
                        case 0xC8: /* SET 1,B */
                        case 0xC9: /* SET 1,C */
                        case 0xCA: /* SET 1,D */
                        case 0xCB: /* SET 1,E */
                        case 0xCC: /* SET 1,H */
                        case 0xCD: /* SET 1,L */
                        case 0xCE: /* SET 1,(HL) */
                        case 0xCF: /* SET 1,A */

                        case 0xD0: /* SET 2,B */
                        case 0xD1: /* SET 2,C */
                        case 0xD2: /* SET 2,D */
                        case 0xD3: /* SET 2,E */
                        case 0xD4: /* SET 2,H */
                        case 0xD5: /* SET 2,L */
                        case 0xD6: /* SET 2,(HL) */
                        case 0xD7: /* SET 2,A */
                        case 0xD8: /* SET 3,B */
                        case 0xD9: /* SET 3,C */
                        case 0xDA: /* SET 3,D */
                        case 0xDB: /* SET 3,E */
                        case 0xDC: /* SET 3,H */
                        case 0xDD: /* SET 3,L */
                        case 0xDE: /* SET 3,(HL) */
                        case 0xDF: /* SET 3,A */

                        case 0xE0: /* SET 4,B */
                        case 0xE1: /* SET 4,C */
                        case 0xE2: /* SET 4,D */
                        case 0xE3: /* SET 4,E */
                        case 0xE4: /* SET 4,H */
                        case 0xE5: /* SET 4,L */
                        case 0xE6: /* SET 4,(HL) */
                        case 0xE7: /* SET 4,A */
                        case 0xE8: /* SET 5,B */
                        case 0xE9: /* SET 5,C */
                        case 0xEA: /* SET 5,D */
                        case 0xEB: /* SET 5,E */
                        case 0xEC: /* SET 5,H */
                        case 0xED: /* SET 5,L */
                        case 0xEE: /* SET 5,(HL) */
                        case 0xEF: /* SET 5,A */

                        case 0xF0: /* SET 6,B */
                        case 0xF1: /* SET 6,C */
                        case 0xF2: /* SET 6,D */
                        case 0xF3: /* SET 6,E */
                        case 0xF4: /* SET 6,H */
                        case 0xF5: /* SET 6,L */
                        case 0xF6: /* SET 6,(HL) */
                        case 0xF7: /* SET 6,A */
                        case 0xF8: /* SET 7,B */
                        case 0xF9: /* SET 7,C */
                        case 0xFA: /* SET 7,D */
                        case 0xFB: /* SET 7,E */
                        case 0xFC: /* SET 7,H */
                        case 0xFD: /* SET 7,L */
                        case 0xFE: /* SET 7,(HL) */
                        case 0xFF: /* SET 7,A */
                            break;
                    }
                }

                /* Unimplemented opcodes */
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
                printf("Unrecognized opcode!");
                break;
        }
    }
}

static void illegal()
{
    sprintf(stderr, "ERROR: Illegal opcode!");
    exit(1); /* Exited with error */
}
