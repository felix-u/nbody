#include <stdint.h>
#include <stdlib.h>
// exact-width integer types
// https://en.wikibooks.org/wiki/C_Programming/inttypes.h
#include <inttypes.h>
#include <stdio.h>
// return explicit error codes
#include <error.h>
// SDL2, my precious
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include "./gravity.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WIDTH 320
#define HEIGHT 180

const uint8_t C_BG[3] = {   0,   0,  32 };
const uint8_t C_FG[3] = { 144,  48, 160 };


int main() {

    // initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

    // create window
    int32_t window_flags =
        SDL_WINDOW_ALLOW_HIGHDPI |
        SDL_WINDOW_INPUT_FOCUS |
        // SDL_WINDOW_RESIZABLE | // enable at least this outside test builds
        // SDL_WINDOW_FULLSCREEN | // if not also this
        SDL_WINDOW_SHOWN;
	SDL_Window* window = SDL_CreateWindow(
        "nbody", // title
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // co-ordinates
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

    // nicer variable names, since we'll be working with them a bit
    // render at this resolution and scale up to SCREEN
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);


    // buffer holding int for logical pixels, every one of which either holds
    // a mass or empty space
    uint_fast8_t mass_buf[WIDTH*HEIGHT] = {0};
    mass_buf[WIDTH*5 + 5] = 1;
    mass_buf[WIDTH*10 + 10] = 1;


    // event loop
    SDL_bool done = SDL_FALSE;
    while (!done) {
        SDL_Event event;

        // draw background
        SDL_SetRenderDrawColor(
            renderer,
            C_BG[0], C_BG[1], C_BG[2],
            SDL_ALPHA_OPAQUE
        );
        SDL_RenderClear(renderer);

        // draw points
        SDL_SetRenderDrawColor(
            renderer,
            C_FG[0], C_FG[1], C_FG[2],
            SDL_ALPHA_OPAQUE
        );
        for (uint32_t pix = 0; pix < WIDTH*HEIGHT; pix++) {
            if (mass_buf[pix] == 1) {
                SDL_RenderDrawPoint(renderer, pix % WIDTH, pix / WIDTH);
            }
        }

        // TODO: keyboard input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            }
        }

        // render
		SDL_RenderPresent(renderer);
	}

    if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
