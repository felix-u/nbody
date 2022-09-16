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

#define LOGICAL_WIDTH 640.
#define LOGICAL_HEIGHT 360.
#define SCALE 2

#define SCREEN_WIDTH LOGICAL_WIDTH*SCALE
#define SCREEN_HEIGHT LOGICAL_HEIGHT*SCALE

const float SCALE_X = SCREEN_WIDTH / LOGICAL_WIDTH;
const float SCALE_Y = SCREEN_HEIGHT / LOGICAL_HEIGHT;

// 16-colour palette
const RGB CLR[16] = {
    {  29,  43,  69 }, // 00 - BLACK
    { 111,  67, 103 }, // 01 - RED
    { 159, 173, 131 }, // 02 - GREEN
    { 240, 177, 115 }, // 03 - YELLOW
    {  86, 104, 143 }, // 04 - BLUE
    { 166, 154, 202 }, // 05 - MAGENTA
    { 123, 151, 157 }, // 06 - CYAN
    { 140, 140, 162 }, // 07 - GREY
    {  52,  52,  97 }, // 08 - LIGHT BLACK
    { 231, 109, 138 }, // 09 - LIGHT RED
    { 201, 201, 154 }, // 10 - LIGHT GREEN
    { 245, 200, 171 }, // 11 - LIGHT YELLOW
    {  37,  54,  87 }, // 12 - LIGHT BLUE
    { 245, 161, 161 }, // 13 - LIGHT MAGENTA
    { 152, 179, 175 }, // 14 - LIGHT CYAN
    { 248, 223, 229 }, // 15 - WHITE
};

int drawCircle(SDL_Renderer *renderer, int x0, int y0, int radius);
int fillCircle(SDL_Renderer *renderer, int x, int y, int radius);

#define TIMESCALE 3
#define DAMPEN_SCALE 4
#define PRINT_ENERGY true


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


    // EVENT LOOP

    SDL_bool done = SDL_FALSE;
    int NOW = SDL_GetPerformanceCounter();
    int LAST = 0;
    double delta = 0;
    // double energy = 0;
    while (!done) {

        // energy = 0;

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        delta = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) * -1;

        SDL_Event event;

        // Draw background
        SDL_SetRenderDrawColor(renderer, CLR[15].r, CLR[15].g, CLR[15].b, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Make sure we have the latest mouse state
        SDL_PumpEvents();

        mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
        win_x = mouse_x / SCALE_X;
        win_y = mouse_y / SCALE_Y;

        // Add body on left click
        SDL_SetRenderDrawColor(renderer, CLR[15].r, CLR[15].g, CLR[15].b, SDL_ALPHA_OPAQUE);
        if (mouse_buttons == 1 && last_frame_mouse_buttons != 1) {
            if (body_num < body_num_max) {

                const RGB *clr = &CLR[15];
                float clr_index = ((float)cursor_radius / (float)cursor_radius_max) * 100.0;
                if (clr_index < 25) clr = &CLR[4];
                else if (clr_index < 50) clr = &CLR[5];
                else if (clr_index < 70) clr = &CLR[9];
                else if (clr_index < 90) clr = &CLR[13];
                else clr = &CLR[11];

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

        for (int i = 0; i < body_num; i++) {

            // Bound to screenspace by "reflecting" on collision - with dampening!
            if (bodies[i].pos_x <= bodies[i].radius) {
                bodies[i].pos_x = bodies[i].radius;
                bodies[i].vel_x = 0 - bodies[i].vel_x / DAMPEN_SCALE;
            }
            else if (bodies[i].pos_x >= (LOGICAL_WIDTH - bodies[i].radius)) {
                bodies[i].pos_x = LOGICAL_WIDTH - bodies[i].radius;
                bodies[i].vel_x = 0 - bodies[i].vel_x / DAMPEN_SCALE;
            }
            if (bodies[i].pos_y <= bodies[i].radius) {
                bodies[i].pos_y = bodies[i].radius;
                bodies[i].vel_y = 0 - bodies[i].vel_y / DAMPEN_SCALE;
            }
            else if (bodies[i].pos_y >= (LOGICAL_HEIGHT - bodies[i].radius)) {
                bodies[i].pos_y = LOGICAL_HEIGHT - bodies[i].radius;
                bodies[i].vel_y = 0 - bodies[i].vel_y / DAMPEN_SCALE;
            }

            // Calculate gravity
            for (int cmp = i + 1; cmp < body_num; cmp++) {

                // @Speed I'm sure there are obvious optimisations I could do here @Speed

                double dist_x = bodies[cmp].pos_x - bodies[i].pos_x;
                double dist_y = bodies[cmp].pos_y - bodies[i].pos_y;
                double dist = sqrt((dist_x * dist_x) + (dist_y * dist_y));

                // @Missing Odd behaviour at close encounters. Consider implementing collisions and having the larger
                // body "eat" the smaller one upon impact @Missing

                double force = 0;
                if (dist >= (bodies[i].radius + bodies[cmp].radius) / 2.0) {
                    force = forceBetween(dist, bodies[i].mass, bodies[cmp].mass) * delta * TIMESCALE;
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

                // energy += 2 * force * dist;
            }
            // energy += 0.5 * bodies[i].mass * ( pow(bodies[i].vel_x, 2) + pow(bodies[i].vel_y, 2) );

            // Increment position by velocity
            bodies[i].pos_x += bodies[i].vel_x;
            bodies[i].pos_y += bodies[i].vel_y;

            // Draw bodies
            SDL_SetRenderDrawColor(renderer, bodies[i].clr.r, bodies[i].clr.g, bodies[i].clr.b, SDL_ALPHA_OPAQUE);
            fillCircle(renderer, bodies[i].pos_x, bodies[i].pos_y, bodies[i].radius);

        }

        // SDL_Log("Total energy: %0.2f\n", energy);

        // Draw cursor
        SDL_SetRenderDrawColor(renderer, CLR[0].r, CLR[0].g, CLR[0].r, SDL_ALPHA_OPAQUE);
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
