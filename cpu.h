/* cpu.h
 * CPU header
 *
 */

#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

/* This union recreates the capabilities of the Game Boy's register set. The CPU
 * has eight 8-bit registers, but some of them can be "combined" to form a
 * 16-bit regsiter.
 */
typedef union
{
    struct
    {
        uint8_t lo_byte;
        uint8_t hi_byte;
    } eight_reg;
    uint16_t sixteen_reg;
} reg_pair_t;

typedef struct
{
    uint8_t A;             /* Accumulator */
    uint8_t F;             /* Flags register */
    reg_pair_t BC, DE, HL; /* General purpose registers */
    uint16_t SP;           /* Stack pointer */
    uint16_t PC;           /* Program counter */

    int iperiod, icount;
    int ibackup;
    uint16_t  irequest;
    uint8_t iautoreset;
} CPU;
