#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

#define BACKGROUND (Color){   5,  31,  57, 255 }
#define FOREGROUND (Color){ 197,  58, 157, 255 }
#define GREY 	   (Color){  74,  36, 128, 255 }
#define BRIGHT     (Color){ 255, 142, 128, 255 }

typedef struct Body {
    Vector2 pos;
    float vel_x;
    float vel_y;
    int radius;
    Color clr;
} Body;

Body InitBody(Vector2 pos, int radius, float vel_x, float vel_y, Color clr);

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    const int SCREENWIDTH = 1920;
    const int SCREENHEIGHT = 1080;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "nbody");

    // it's only this high for development, so I can get a better idea of what's
    // taxing the game the most. This should be my monitor's refresh rate for
    // actual gameplay
    SetTargetFPS(1000);
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

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        /* // Update */
        /* //---------------------------------------------------------------------------------- */

        // get mouse position
        Vector2 mousePos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && bodyNum < 10) {
            bodies[bodyNum] = InitBody(mousePos, candidateRadius, 0, 0, FOREGROUND);
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
        /* //---------------------------------------------------------------------------------- */

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BACKGROUND);

        // draw target ring under circle
        DrawRing(mousePos, candidateRadius-3, candidateRadius, 0, 360, candidateRadius, BRIGHT);

        for (int i = 0; i < bodyNum; i++) {
            Body b = bodies[i];
            DrawCircleV(b.pos, b.radius, b.clr);
        }

        sprintf(textFPS, "%d", GetFPS());
        DrawText(textFPS, SCREENWIDTH/20, SCREENHEIGHT/20, 25, GREY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }


    CloseWindow();
    return 0;
}

Body InitBody(Vector2 pos, int radius, float vel_x, float vel_y, Color clr) {
    Body body;
    body.pos = pos;
    body.radius = radius;
    body.vel_x = vel_x;
    body.vel_y = vel_y;
    body.clr = clr;

    return body;
}
