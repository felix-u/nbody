#include <stdint.h>
#include <stdlib.h>
// exact-width integer types
// https://en.wikibooks.org/wiki/C_Programming/inttypes.h
#include <inttypes.h>
#include <stdio.h>
// return explicit error codes
#include <error.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 180


int main() {

    // initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

    // create window
    // NOTE: Using SDL_WINDOWPOS_CENTERED will *VERY STUPIDLY* silently force
    //       X11 over Wayland, because Wayland doesn't expose window
    //       positioning functionality to clients. So I'm not using it...
    //       OBVIOUSLY.
	SDL_Window* window = SDL_CreateWindow(
        "nbody", // title
        100, 100, // co-ordinates (see note above)
        SCREEN_WIDTH, SCREEN_HEIGHT, // dimensions
        SDL_WINDOW_SHOWN
    );
	if (window == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

    // create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);

    // NOTE: Can't use SDL_Event because VIDEODRIVER=x11 will be forced
    SDL_bool done = SDL_FALSE;
    while (!done) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 32, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 144, 48, 160, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, 20, 40, 20, 80);

		SDL_RenderPresent(renderer);
	}

    if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

