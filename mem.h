/* mem.h
 * Memory interface
 */

#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>

#define MEM_SIZE 0x10000

extern uint8_t memory[MEM_SIZE];

void mem_write(uint16_t addr, uint8_t data);
uint8_t mem_read(uint16_t addr);

#endif /* _MEM_H_ */
