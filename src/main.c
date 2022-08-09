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
#include <SDL2/SDL_hints.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 180

// FIXME
// obviously I'm doing something here wrong, since the C99 example from
// https://github.com/xyproto/sdl2-examples works with wayland, and this
// doesn't


int main() {

    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("We compiled against SDL version %u.%u.%u ...\n",
           compiled.major, compiled.minor, compiled.patch);
    printf("But we are linking against SDL version %u.%u.%u.\n",
           linked.major, linked.minor, linked.patch);

    // initialise SDL
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "wayland");
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
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);

    // event loop
    SDL_bool done = SDL_FALSE;
    while (!done) {
        SDL_Event event;

        SDL_SetRenderDrawColor(renderer, 0, 0, 32, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 144, 48, 160, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, 20, 40, 20, 80);

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

