/* interrupt.c
 * Memory implementation
 */

#include "mem.h"
#include "interrupt.h"

void interrupt_enable(void)
{
    mem_write(0xFFFF, 0xFF);
}

void interrupt_disable(void)
{
    mem_write(0xFFFF, 0x00);
}
