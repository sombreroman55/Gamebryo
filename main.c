/* main.c
 * The main entry point for the emulator program.
 */

//#include "system.h"
#include <SDL2/SDL.h>
#include "cpu.h"

int main (int argc, char** argv)
{
    CPU cpu;
    cpu_init(&cpu);
    return 0;
}
