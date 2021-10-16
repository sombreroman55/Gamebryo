/* main.c
 * The main entry point for the emulator program.
 */

//#include "system.h"
#include <SDL2/SDL.h>

int main (int argc, char** argv)
{
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("We compiled against SDL version %d.%d.%d ...\n",
                   compiled.major, compiled.minor, compiled.patch);
    printf("But we are linking against SDL version %d.%d.%d.\n",
                   linked.major, linked.minor, linked.patch);
    
    return 0;
}
