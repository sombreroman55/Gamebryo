/* system.c
 * Implements the virtual Game Boy system
 */

#include "system.h"

System* initialize_system(void)
{
    System* system;
    system = malloc(sizeof(System));
    return system;
}

void play(System* sys)
{
    run(&sys->cpu)
}

void turn_off(System* sys)
{
    free(sys);
}
