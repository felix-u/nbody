#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include <math.h>

#define BACKGROUND (Color){   0,  48,  59, 255 }
#define DARKGREY   (Color){ 255, 119, 119, 255 }
#define LIGHTGREY  (Color){ 255, 206, 150, 255 }
#define FOREGROUND (Color){ 241, 242, 218, 255 }

// this may or may not be scaled - actual value is of magnitude E-11
#define G_NEWTON   (6.6743 * pow(10, -6))

typedef struct Body {
    Vector2 pos;
    double vel_x;
    double vel_y;
    int radius;
    Color clr;
    long int mass;
} Body;

Body InitBody(Vector2 pos, int radius, float vel_x, float vel_y, Color clr, int mass);

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    const int SCREENWIDTH = 1280;
    const int SCREENHEIGHT = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "nbody");

    SetTargetFPS(120);
    //---------------------------------------------------------------------------------------

    // INITIAL VARIABLES

    // for scrolling to change candidate size
    const int SENSITIVITY = 2;
    int candidateRadius = 10;
    int inputCurve = 1;

    // for displaying FPS
    char textFPS[5];

    // le gravitation
    const int MAX_BODIES = 10;
    Body bodies[MAX_BODIES];
    int bodyNum = 0;
    const long double PIXELS_PER_METRE = 100;
    const long int MASS_PER_SQUARE_PIXEL = 100000;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        /* // Update */
        /* //---------------------------------------------------------------------------------- */

        // get mouse position
        Vector2 mousePos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && bodyNum < 10) {
            bodies[bodyNum] = InitBody(mousePos, candidateRadius, 0, 0,
                FOREGROUND, pow(candidateRadius*PIXELS_PER_METRE, 2) * MASS_PER_SQUARE_PIXEL);
            bodyNum++;
        }

        // control curve - when candidate radius smaller, scroll makes smaller
        // adjustment. when radius larger, scroll makes larger adjustment.
        int scroll = GetMouseWheelMove();
        if (scroll) {
            inputCurve = candidateRadius / 10;
            if (inputCurve < 1) inputCurve = 1;
            candidateRadius += (scroll * inputCurve * SENSITIVITY);
            if (candidateRadius < 5) candidateRadius = 5;
        }

        // n-body calculations
        long double delta = GetFrameTime();
        for (int i = 0; i < bodyNum; i++) {
            long double x_iAccel, y_iAccel;
            for (int comp = 0; comp < i; comp++) if (i != comp) {
                long double x_distance = ( bodies[i].pos.x - bodies[comp].pos.x ) / PIXELS_PER_METRE;
                long double y_distance = ( bodies[i].pos.y - bodies[comp].pos.y ) / PIXELS_PER_METRE;
                long double collisionDistance = (bodies[i].radius + bodies[comp].radius) / PIXELS_PER_METRE;
                long double distance = sqrt(pow(x_distance, 2) + pow(y_distance, 2));
                long double angleItoComp = atan2(y_distance, x_distance);

                long double force;
                // if (distance < 1) distance = 1;
                force = G_NEWTON * bodies[i].mass * bodies[comp].mass
                                / pow(distance, 2);
                long double x_force = force * cosl(angleItoComp);
                long double y_force = force * sinl(angleItoComp);
                // printf("Body %d feeling x force %0.2LfN, y force %0.2LfN towards body %d\n", i+1, x_force, y_force, comp+1);

                long double x_iAccel = x_force / bodies[i].mass;
                long double y_iAccel = y_force / bodies[i].mass;
                long double x_compAccel = x_force / bodies[comp].mass;
                long double y_compAccel = y_force / bodies[comp].mass;
                // printf("Body %d x accel %0.10Lfm/s, y accel %0.10Lfm/s due to body %d\n", i+1, x_iAccel, y_iAccel, comp+1);

                if (distance > collisionDistance) {
                    if (x_distance > 0) {
                        bodies[i].pos.x    -= fabsl(x_iAccel) * pow(delta, 2);
                        bodies[comp].pos.x += fabsl(x_compAccel) * pow(delta, 2);
                    }
                    else {
                        bodies[i].pos.x    += fabsl(x_iAccel) * pow(delta, 2);
                        bodies[comp].pos.x -= fabsl(x_compAccel) * pow(delta, 2);
                    }
                    if (y_distance > 0) {
                        bodies[i].pos.y    -= fabsl(y_iAccel) * pow(delta, 2);
                        bodies[comp].pos.y += fabsl(y_compAccel) * pow(delta, 2);
                    }
                    else {
                        bodies[i].pos.y    += fabsl(y_iAccel) * pow(delta, 2);
                        bodies[comp].pos.y -= fabsl(y_compAccel) * pow(delta, 2);
                    }
                }

                printf("body %d at y dist %0.2Lfm,", i+1, y_distance);
                printf(" angle %0.2Lf from body %d", angleItoComp, comp+1);
                printf(" feeling %0.0LfN x, %0.0LfN y)\r", x_force, y_force);
                putchar('\n');
            }
            // printf("Body %d has x accel %0.10Lfm/s^2,", i+1, x_iAccel);
            // printf(" y accel %0.10Lfm/s^2\n", y_iAccel);
        }

        /* //---------------------------------------------------------------------------------- */

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BACKGROUND);

        // draw bodies
        for (int i = 0; i < bodyNum; i++) {
            Body b = bodies[i];
            // b.pos.x += b.vel_x / PIXELS_PER_METRE;
            // b.pos.y += b.vel_y / PIXELS_PER_METRE;
            DrawCircleV(b.pos, b.radius, b.clr);
        }

        sprintf(textFPS, "%d", GetFPS());
        DrawText(textFPS, SCREENWIDTH/20, SCREENHEIGHT/20, 25, LIGHTGREY);

        // draw target ring under circle
        DrawRing(mousePos, candidateRadius-3, candidateRadius, 0, 360, candidateRadius, LIGHTGREY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }


    CloseWindow();
    return 0;
}

Body InitBody(Vector2 pos, int radius, float vel_x, float vel_y, Color clr, int mass) {
    Body body;
    body.pos = pos;
    body.radius = radius;
    body.vel_x = vel_x;
    body.vel_y = vel_y;
    body.clr = clr;
    body.mass = mass;

    return body;
}
