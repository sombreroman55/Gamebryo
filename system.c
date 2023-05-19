/* system.c
 * Implements the virtual Game Boy system
 */

#include <stdlib.h>
#include "system.h"

System* system_init(void)
{
    // TODO: Initialize other components
    System* system = (System*)malloc(sizeof(System));
    system->cpu = cpu_init();
    return system;
}

void system_tick(System* sys)
{
    cpu_tick(sys->cpu);
}
