#include "SDL2/SDL_render.h"
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

int main() {

    // initialise SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
            return EXIT_FAILURE;
	}

    // create window
    SDL_Window* win = SDL_CreateWindow(
        "nbody",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1920, 1080,
        SDL_WINDOW_SHOWN
    );
    if (win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
            return EXIT_FAILURE;
    }

    // create renderer, which sets up hardware acceleration
    SDL_Renderer* ren = SDL_CreateRenderer(
        win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (ren == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}

