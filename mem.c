/* mem.c
 * Memory implementation
 */

#include "mem.h"

uint8_t memory[0x10000] = {0};

inline void mem_write(uint8_t addr, uint8_t byte)
{
    memory[addr] = byte;
}

inline uint8_t mem_read(uint8_t addr)
{
    return memory[addr];
}
