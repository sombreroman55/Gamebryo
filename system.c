/* system.c
 * Implements the virtual Game Boy system
 */

#include "system.h"

void initialize_system(System* system)
{
}

void play(System* sys)
{
    run(&sys->cpu)
}

void turn_off(System* sys)
{
    free(sys);
}
