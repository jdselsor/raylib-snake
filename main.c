#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main () {
    InitWindow (SCREEN_WIDTH, SCREEN_HEIGHT, "TITLE");
    SetTargetFPS (60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText ("HELLO WORLD!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}