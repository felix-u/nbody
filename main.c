#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include <math.h>

#define BACKGROUND (Color){   5,  31,  57, 255 }
#define FOREGROUND (Color){ 197,  58, 157, 255 }
#define GREY 	   (Color){  74,  36, 128, 255 }
#define BRIGHT     (Color){ 255, 142, 128, 255 }

// this may or may not be scaled - actual value is of magnitude E-11
#define G_NEWTON   (6.6743 * pow(10, -11))

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
    const int PIXELS_PER_METRE = 10;
    const long int MASS_PER_SQUARE_PIXEL = 10000;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        /* // Update */
        /* //---------------------------------------------------------------------------------- */

        // get mouse position
        Vector2 mousePos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && bodyNum < 10) {
            bodies[bodyNum] = InitBody(mousePos, candidateRadius, 0, 0,
                FOREGROUND, pow(candidateRadius, 2) * MASS_PER_SQUARE_PIXEL);
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
        float delta = GetFrameTime();
        for (int i = 0; i < bodyNum; i++) {
            for (int comp = 0; comp < i; comp++) if (i != comp) {
                double x_distance = ( bodies[i].pos.x - bodies[comp].pos.x ) / PIXELS_PER_METRE;
                double y_distance = ( bodies[i].pos.y - bodies[comp].pos.y ) / PIXELS_PER_METRE;
                double distance = sqrt(pow(x_distance, 2) + pow(y_distance, 2));
                double angleItoComp = atan2(y_distance, x_distance);

                long double force = G_NEWTON * bodies[i].mass * bodies[comp].mass
                                / pow(distance, 2);
                long double x_force = force * cosl(angleItoComp);
                long double y_force = force * sinl(angleItoComp);

                printf("body %d at dist %0.2fm,", i+1, distance);
                printf(" angle %0.2f from body %d", angleItoComp, comp+1);
                printf(" feeling %0.3LfN x, %0.3LfN y)\n", x_force, y_force);
            }
        }

        /* //---------------------------------------------------------------------------------- */

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BACKGROUND);

        // draw bodies
        for (int i = 0; i < bodyNum; i++) {
            Body b = bodies[i];
            b.pos.x += b.vel_x;
            b.pos.y += b.vel_y;
            DrawCircleV(b.pos, b.radius, b.clr);
        }

        sprintf(textFPS, "%d", GetFPS());
        DrawText(textFPS, SCREENWIDTH/20, SCREENHEIGHT/20, 25, GREY);

        // draw target ring under circle
        DrawRing(mousePos, candidateRadius-3, candidateRadius, 0, 360, candidateRadius, BRIGHT);

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
