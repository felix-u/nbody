#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

#define BACKGROUND (Color){ 147, 161, 161, 255 }
#define FOREGROUND (Color){  34,  71,  80, 255 }

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "nbody");

    SetTargetFPS(60);
    //---------------------------------------------------------------------------------------

    // INITIAL VARIABLES

    // for scrolling to change candidate size
    int candidateRadius = 10;
    int scroll = 0;
    int inputScale = 1;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
	/* // Update */
	/* //---------------------------------------------------------------------------------- */

	/* if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ballColor = MAROON; */
	/* else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) ballColor = LIME; */
	/* else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) ballColor = DARKBLUE; */
	/* else if (IsMouseButtonPressed(MOUSE_BUTTON_SIDE)) ballColor = PURPLE; */
	/* else if (IsMouseButtonPressed(MOUSE_BUTTON_EXTRA)) ballColor = YELLOW; */
	/* else if (IsMouseButtonPressed(MOUSE_BUTTON_FORWARD)) ballColor = ORANGE; */
	/* else if (IsMouseButtonPressed(MOUSE_BUTTON_BACK)) ballColor = BEIGE; */
	/* //---------------------------------------------------------------------------------- */

	// Draw
	//----------------------------------------------------------------------------------
	BeginDrawing();
	ClearBackground(BACKGROUND);

	// control curve - when candidate radius smaller, scroll makes smaller
	// adjustment. when radius larger, scroll makes larger adjustment.
	scroll = GetMouseWheelMove();
	if (GetMouseWheelMove()) {
	    inputScale = candidateRadius / 10;
	    if (inputScale < 1) inputScale = 1;
	    candidateRadius += (scroll * inputScale);   
	    if (candidateRadius < 5) candidateRadius = 5;
	}
	DrawCircleV(GetMousePosition(), candidateRadius, FOREGROUND);

	/* DrawText("move ball with mouse and click mouse button to change color", 10, 10, 20, FOREGROUND); */

	EndDrawing();
	//----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
