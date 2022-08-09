#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
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

const uint8_t C_BG[3] = {   0,   0,  32 };
const uint8_t C_FG[3] = { 144,  48, 160 };

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
    int32_t window_flags =
        SDL_WINDOW_ALLOW_HIGHDPI |
        SDL_WINDOW_INPUT_FOCUS |
        SDL_WINDOW_SHOWN;
	SDL_Window* window = SDL_CreateWindow(
        "nbody", // title
        100, 100, // co-ordinates (see note above)
        SCREEN_WIDTH, SCREEN_HEIGHT, // dimensions
        window_flags
    );
	if (window == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

    // create renderer
    uint8_t render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);
	if (renderer == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

    // render at THIS resolution and scale up to SCREEN
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);

    SDL_bool done = SDL_FALSE;
    while (!done) {
        SDL_Event event;

        SDL_SetRenderDrawColor(renderer, C_BG[0], C_BG[1], C_BG[2], SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, C_FG[0], C_FG[1], C_FG[2], SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, 20, 40, 20, 80);
		SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            }
        }
	}

    if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
