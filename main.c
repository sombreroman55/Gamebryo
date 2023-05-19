/* main.c
 * The main entry point for the emulator program.
 */

//#include "system.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "system.h"

int main (int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("OpenGL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    SDL_Event event;
    int quit = 0;

    System* system = system_init();

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            // TODO: Handle input
        }

        system_tick(system);

        // TODO: Render the screen

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Render your OpenGL graphics here

        // Swap the back buffer with the front buffer
        SDL_GL_SwapWindow(window);
    }

    return 0;
}
