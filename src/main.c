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
#include <SDL2/SDL_timer.h>

#include "./gravity.c"

#define PI 3.142857

#define SCREEN_WIDTH 1600.
#define SCREEN_HEIGHT 900.
#define LOGICAL_WIDTH 640.
#define LOGICAL_HEIGHT 360.

const float SCALE_X = SCREEN_WIDTH / LOGICAL_WIDTH;
const float SCALE_Y = SCREEN_HEIGHT / LOGICAL_HEIGHT;

const RGB CLR_BG     = {  20,  31,  64 };
const RGB CLR_BLUE   = {  93, 161, 224 };
const RGB CLR_PURPLE = { 126, 112, 205 };
const RGB CLR_PINK   = { 198, 131, 211 };
const RGB CLR_RED    = { 198,  93, 130 };
const RGB CLR_ORANGE = { 220, 169, 123 };
const RGB CLR_YELLOW = { 205, 172, 112 };
const RGB CLR_FG     = { 202, 193, 251 };

int drawCircle(SDL_Renderer *renderer, int x0, int y0, int radius);
int fillCircle(SDL_Renderer *renderer, int x, int y, int radius);

#define TIMESCALE 3;


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
    const int cursor_radius_min = 2;

    SDL_ShowCursor(SDL_DISABLE);

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
    int NOW = SDL_GetPerformanceCounter();
    int LAST = 0;
    double delta = 0;
    while (!done) {

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        delta = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) * -1;

        SDL_Event event;

        // Draw background
        SDL_SetRenderDrawColor(renderer, CLR_BG.r, CLR_BG.g, CLR_BG.b, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Make sure we have the latest mouse state
        SDL_PumpEvents();

        mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
        win_x = mouse_x / SCALE_X;
        win_y = mouse_y / SCALE_Y;

        // Add body on left click
        SDL_SetRenderDrawColor(renderer, CLR_FG.r, CLR_FG.g, CLR_FG.b, SDL_ALPHA_OPAQUE);
        if (mouse_buttons == 1 && last_frame_mouse_buttons != 1) {
            if (body_num < body_num_max) {

                const RGB *clr = &CLR_FG;
                float clr_index = ((float)cursor_radius / (float)cursor_radius_max) * 100.0;
                if (clr_index < 25) clr = &CLR_BLUE;
                else if (clr_index < 50) clr = &CLR_PURPLE;
                else if (clr_index < 70) clr = &CLR_RED;
                else if (clr_index < 90) clr = &CLR_ORANGE;
                else clr = &CLR_YELLOW;

                int rad = cursor_radius - 1;

                Body new_body = {
                    win_x, win_y,
                    0.0, 0.0,
                    rad,
                    rad * rad * rad * (4.0 / 3.0) * PI,
                    *clr
                };
                bodies[body_num] = new_body;
                body_num++;
            }
            else SDL_Log("Reached max number of bodies: %d\n", body_num);
        }

        // Calculate gravity
        for (int i = 0; i < body_num; i++) {
            for (int cmp = i + 1; cmp < body_num; cmp++) {

                // @Speed I'm sure there are obvious optimisations I could do here @Speed

                double dist_x = bodies[cmp].pos_x - bodies[i].pos_x;
                double dist_y = bodies[cmp].pos_y - bodies[i].pos_y;
                double dist = sqrt((dist_x * dist_x) + (dist_y * dist_y));

                // @Missing Odd behaviour at close encounters. Consider implementing collisions and having the larger
                // body "eat" the smaller one upon impact @Missing

                if (dist >= (bodies[i].radius + bodies[cmp].radius) / 2.0) {
                    double force = forceBetween(dist, bodies[i].mass, bodies[cmp].mass) * delta * TIMESCALE;
                    double body_accel = force / bodies[i].mass;
                    double body_cmp_accel = force / bodies[cmp].mass;
                    double body_accel_x = body_accel * (dist_x / dist);
                    double body_accel_y = body_accel * (dist_y / dist);
                    double body_cmp_accel_x = body_cmp_accel * (dist_x / dist) * -1;
                    double body_cmp_accel_y = body_cmp_accel * (dist_y / dist) * -1;

                    bodies[i].vel_x += body_accel_x;
                    bodies[i].vel_y += body_accel_y;
                    bodies[cmp].vel_x += body_cmp_accel_x;
                    bodies[cmp].vel_y += body_cmp_accel_y;
                }
            }

            // Increment position by velocity
            bodies[i].pos_x += bodies[i].vel_x;
            bodies[i].pos_y += bodies[i].vel_y;

            // Bound to screenspace by "reflecting" on collision - with dampening!
            if (bodies[i].pos_x <= bodies[i].radius) {
                bodies[i].pos_x = bodies[i].radius;
                bodies[i].vel_x = 0 - bodies[i].vel_x / 4;
            }
            else if (bodies[i].pos_x >= (LOGICAL_WIDTH - bodies[i].radius)) {
                bodies[i].pos_x = LOGICAL_WIDTH - bodies[i].radius;
                bodies[i].vel_x = 0 - bodies[i].vel_x / 4;
            }
            if (bodies[i].pos_y <= bodies[i].radius) {
                bodies[i].pos_y = bodies[i].radius;
                bodies[i].vel_y = 0 - bodies[i].vel_y / 4;
            }
            else if (bodies[i].pos_y >= (LOGICAL_HEIGHT - bodies[i].radius)) {
                bodies[i].pos_y = LOGICAL_HEIGHT - bodies[i].radius;
                bodies[i].vel_y = 0 - bodies[i].vel_y / 4;
            }

            // Draw bodies
            SDL_SetRenderDrawColor(renderer, bodies[i].clr.r, bodies[i].clr.g, bodies[i].clr.b, SDL_ALPHA_OPAQUE);
            fillCircle(renderer, bodies[i].pos_x, bodies[i].pos_y, bodies[i].radius);

        }

        // Draw cursor
        SDL_SetRenderDrawColor(renderer, CLR_PINK.r, CLR_PINK.g, CLR_PINK.r, SDL_ALPHA_OPAQUE);
        drawCircle(renderer, win_x, win_y, cursor_radius);


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


// Midpoint circle algorithm

int drawCircle(SDL_Renderer *renderer, int x0, int y0, int radius) {
    int x, y, d;
    int err;

    x = 0;
    y = radius;
    d = radius - 1;
    err = 0;

    while (y >= x) {
        err += SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        err += SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
        err += SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
        err += SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
        err += SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);
        err += SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
        err += SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
        err += SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);

        if (err < 0) {
            err = -1;
            break;
        }

        if (d >= 2*x) {
            d -= 2*x + 1;
            x +=1;
        }
        else if (d < 2 * (radius - y)) {
            d += 2 * y - 1;
            y -= 1;
        }
        else {
            d += 2 * (y - x - 1);
            y -= 1;
            x += 1;
        }
    }

    return err;
}


int fillCircle(SDL_Renderer *renderer, int x, int y, int radius) {
    int x_offset, y_offset, d;
    int err;

    x_offset = 0;
    y_offset = radius;
    d = radius -1;
    err = 0;

    while (y_offset >= x_offset) {

        err += SDL_RenderDrawLine(renderer, x - y_offset, y + x_offset, x + y_offset, y + x_offset);
        err += SDL_RenderDrawLine(renderer, x - x_offset, y + y_offset, x + x_offset, y + y_offset);
        err += SDL_RenderDrawLine(renderer, x - x_offset, y - y_offset, x + x_offset, y - y_offset);
        err += SDL_RenderDrawLine(renderer, x - y_offset, y - x_offset, x + y_offset, y - x_offset);

        if (err < 0) {
            err = -1;
            break;
        }

        if (d >= 2*x_offset) {
            d -= 2*x_offset + 1;
            x_offset +=1;
        }
        else if (d < 2 * (radius - y_offset)) {
            d += 2 * y_offset - 1;
            y_offset -= 1;
        }
        else {
            d += 2 * (y_offset - x_offset - 1);
            y_offset -= 1;
            x_offset += 1;
        }
    }

    return err;
}
