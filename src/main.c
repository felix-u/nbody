#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <error.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_mouse.h>

#include "./gravity.c"

#define PI 3.142857

#define SCREEN_WIDTH 1600.
#define SCREEN_HEIGHT 900.
#define LOGICAL_WIDTH 640.
#define LOGICAL_HEIGHT 360.

const float SCALE_X = SCREEN_WIDTH / LOGICAL_WIDTH;
const float SCALE_Y = SCREEN_HEIGHT / LOGICAL_HEIGHT;

const uint8_t CLR_BG[3] = {  20,  31,  64 };
const uint8_t CLR_FG[3] = { 202, 193, 251 };


int main() {

    // Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

    // Create window
    int32_t window_flags =
        SDL_WINDOW_ALLOW_HIGHDPI |
        SDL_WINDOW_INPUT_FOCUS |
        // SDL_WINDOW_RESIZABLE | // Enable at least this outside test builds
        // SDL_WINDOW_FULLSCREEN | // if not also this
        SDL_WINDOW_SHOWN;
	SDL_Window* window = SDL_CreateWindow(
        // Title
        "nbody",
        // Coordinates
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        // Dimensions
        SCREEN_WIDTH, SCREEN_HEIGHT,
        window_flags
    );
	if (window == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

    // Create renderer
    uint8_t render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);
	if (renderer == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

    // Render at THIS resolution and scale up to SCREEN
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);


    // GAME SETUP

    int mouse_x = 0, mouse_y = 0, win_x = 0, win_y = 0;
    uint32_t mouse_buttons = 0, last_frame_mouse_buttons = 0;

    const int body_num_max = 100;
    Body bodies[body_num_max];
    int body_num = 0;

    int cursor_radius = 2;
    const int cursor_radius_max = 20;
    const int cursor_radius_min = 1;

    // // const int default_body_radius = cursor_radius_max * 2;
    // const int default_body_radius = 2;
    // Body default_body = {
    //     LOGICAL_WIDTH / 2.0,    // pos_x
    //     LOGICAL_HEIGHT / 2.0,   // pos_y
    //     1.0,                    // vel_x
    //     0.0,                    // vel_y
    //     default_body_radius,    // radius
    //     default_body_radius * default_body_radius * default_body_radius * (4.0 / 3.0) * PI,  // mass
    // };
    // bodies[0] = default_body;


    // EVENT LOOP

    SDL_bool done = SDL_FALSE;
    while (!done) {
        SDL_Event event;

        // Draw background
        SDL_SetRenderDrawColor(renderer, CLR_BG[0], CLR_BG[1], CLR_BG[2], SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Make sure we have the latest mouse state
        SDL_PumpEvents();

        mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
        win_x = mouse_x / SCALE_X;
        win_y = mouse_y / SCALE_Y;

        // Draw bodies
        SDL_SetRenderDrawColor(renderer, CLR_FG[0], CLR_FG[1], CLR_FG[2], SDL_ALPHA_OPAQUE);
        for (int i = 0; i < body_num; i++) {

            // Increment position by velocity
            bodies[i].pos_x += bodies[i].vel_x;
            bodies[i].pos_y += bodies[i].vel_y;

            // Render body
            const SDL_Rect body = {
                bodies[i].pos_x - bodies[i].radius,
                bodies[i].pos_y - bodies[i].radius,
                bodies[i].radius * 2,
                bodies[i].radius * 2
            };
            SDL_RenderFillRect(renderer, &body);
        }

        // Draw cursor
        SDL_SetRenderDrawColor(renderer, CLR_FG[0], CLR_FG[1], CLR_FG[2], SDL_ALPHA_OPAQUE);
        SDL_Rect cursor = {
            win_x - cursor_radius,
            win_y - cursor_radius,
            cursor_radius * 2,
            cursor_radius * 2
        };
        SDL_RenderDrawRect(renderer, &cursor);

        // Add body on left click
        SDL_SetRenderDrawColor(renderer, CLR_FG[0], CLR_FG[1], CLR_FG[2], SDL_ALPHA_OPAQUE);
        if (mouse_buttons == 1 && last_frame_mouse_buttons != 1) {
            if (body_num < body_num_max) {
                Body new_body = {
                    win_x, win_y,
                    0.0, 0.0,
                    cursor_radius,
                    cursor_radius * cursor_radius * cursor_radius * (4.0 / 3.0) * PI,
                };
                bodies[body_num] = new_body;
                body_num++;
            }
            else SDL_Log("Reached max number of bodies: %d\n", body_num);
        }

        // Calculate gravity
        for (int body = 0; body < body_num; body++) {
            for (int body_cmp = body + 1; body_cmp < body_num; body_cmp++) {

                // @Speed I'm sure there are obvious optimisations I could do here @Speed

                double dist_x = bodies[body_cmp].pos_x - bodies[body].pos_x;
                double dist_y = bodies[body_cmp].pos_y - bodies[body].pos_y;
                double dist = sqrt((dist_x * dist_x) + (dist_y * dist_y));

                // @Missing Odd behaviour at close encounters. Consider implementing collisions and having the larger
                // body "eat" the smaller one upon impact @Missing

                if (dist >= 10) {
                    // double angle = atan2(dist_y, dist_x);

                    double force = forceBetween(dist, bodies[body].mass, bodies[body_cmp].mass);
                    double body_accel = force / bodies[body].mass;
                    double body_cmp_accel = force / bodies[body_cmp].mass;
                    double body_accel_x = body_accel * (dist_x / dist);
                    double body_accel_y = body_accel * (dist_y / dist);
                    double body_cmp_accel_x = body_cmp_accel * (dist_x / dist) * -1;
                    double body_cmp_accel_y = body_cmp_accel * (dist_y / dist) * -1;

                    bodies[body].vel_x += body_accel_x;
                    bodies[body].vel_y += body_accel_y;
                    bodies[body_cmp].vel_x += body_cmp_accel_x;
                    bodies[body_cmp].vel_y += body_cmp_accel_y;

                    // SDL_Log("Body %d and %d are separated at angle %0.2f\n", body, body_cmp, angle);
                }



            }
        }

        while (SDL_PollEvent(&event)) {

            // Let user exit
            if (event.type == SDL_QUIT) done = SDL_TRUE;

            // Modify size of cursor on scroll
            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0 && cursor_radius < cursor_radius_max) {
                    cursor_radius += 1;
                }
                else if (event.wheel.y < 0 && cursor_radius > cursor_radius_min) {
                    cursor_radius -= 1;
                }
            }
        }

        // Render
		SDL_RenderPresent(renderer);

        last_frame_mouse_buttons = mouse_buttons;
	}


    if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
