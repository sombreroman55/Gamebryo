/* system.h
 * Represents the virtual Game Boy system to be emulated
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "cpu.h"

typedef struct
{
   CPU* cpu;
} System;

System* system_init(void);
void system_tick(System* sys);

#endif /* _SYSTEM_H_ */
