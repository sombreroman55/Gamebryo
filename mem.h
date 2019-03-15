/* mem.h
 * Memory interface
 */

#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>

extern uint8_t memory[0x10000];

inline void mem_write(uint8_t addr, uint8_t data);
inline uint8_t mem_read(uint8_t addr);

#endif /* _MEM_H_ */
