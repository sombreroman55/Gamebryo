/* mem.c
 * Memory implementation
 */

#include "mem.h"

uint8_t memory[MEM_SIZE] = {0};

inline void mem_write(uint16_t addr, uint8_t byte)
{
    memory[addr] = byte;
}

inline uint8_t mem_read(uint16_t addr)
{
    return memory[addr];
}
