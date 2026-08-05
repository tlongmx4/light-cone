#include "raylib.h"

struct Event {
    double x; // space
    double t; // time
    double z; // space
};

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Light Cone");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = { 20.0f, 15.0f, 20.0f };
    camera.target = { 0.0f, 5.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        DrawGrid(20, 1.0f);
        DrawCubeWires({0, 0, 0}, 720, 720, 720, DARKGRAY);
        EndMode3D();
        EndDrawing();

    }

    CloseWindow();
    return 0;
}