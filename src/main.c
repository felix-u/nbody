#include <stdint.h>
#include <stdlib.h>
// exact-width integer types
// https://en.wikibooks.org/wiki/C_Programming/inttypes.h
#include <inttypes.h>
#include <stdio.h>
// return explicit error codes
#include <error.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main() {

    // initialise SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
            return EXIT_FAILURE;
	}

    // create window
    SDL_Window* window = SDL_CreateWindow(
        "nbody",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
            return EXIT_FAILURE;
    }

    // create renderer, which sets up hardware acceleration
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (renderer == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

    // event loop
    SDL_bool done = SDL_FALSE;
    while (!done) {
        SDL_Event event;

        SDL_SetRenderDrawColor(renderer, 0, 0, 32, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 144, 48, 160, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, 320, 200, 320, 240);
        // SDL_Delay(2000);

        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            }
        }
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    return EXIT_SUCCESS;
}

