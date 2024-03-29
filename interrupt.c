/* interrupt.c
 * Memory implementation
 */

#include "interrupt.h"
#include "mem.h"
#include <stdbool.h>

void interrupt_enable(void) { mem_write(0xFFFF, 0xFF); }

void interrupt_disable(void) { mem_write(0xFFFF, 0x00); }

bool interrupt_pending(void) {  }
