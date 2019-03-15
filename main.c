/* main.c
 * The main entry point for the emulator program.
 */

//#include "system.h"
#include <SDL2/SDL.h>

int main (int argc, char** argv)
{
    system gamebryo;
    int quit = 0;
    while (!quit)
    {
        run(gamebryo);
    }
    return 0;
}
