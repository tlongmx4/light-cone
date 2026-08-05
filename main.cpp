#include "raylib.h"
#include <cmath>
#include <vector>

constexpr double c = 1.0;

struct Event {
    double t; // time
    double x; // space
    double y;
    double z;
};

enum class Causality {
    FutureCone,
    PastCone,
    FutureInterior,
    PastInterior,
    Outside,
    Unknown
};

Causality classify(const Event& from, const Event& to);

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

int main() {

    double v = 0.0; // observer velocity
    std::vector<Event> events;

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
        DrawCubeWires({0, 0, 0}, 20, 20, 20, DARKGRAY);
        EndMode3D();
        EndDrawing();

    }

    CloseWindow();
    return 0;
}