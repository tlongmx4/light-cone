#include <iostream>
#include "raylib.h"
#include <cmath>

struct Cone {
    double x; // space
    double y; // time
    double z; // space
};

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Light Cone");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        EndDrawing();

    }

    CloseWindow();
    return 0;
}