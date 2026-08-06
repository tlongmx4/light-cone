#include "raylib.h"
#include <cmath>
#include <vector>
#include <cstdio>

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

Event boost(const Event& e, double v);
Causality classify(const Event& from, const Event& to) {
    double dt = to.t - from.t;
    double dx = to.x - from.x;
    double dy = to.y - from.y;
    double dz = to.z - from.z;

    double s2 = -(dt * dt) + (dx * dx) + (dy * dy) + (dz * dz);

    if (s2 > 1e-9) return Causality::Outside;
    if (fabs(dt) < 1e-9) return Causality::Unknown;

    if (fabs(s2) < 1e-9) {
        return dt > 0 ? Causality::FutureCone : Causality::PastCone;
    }

    return dt > 0 ? Causality::FutureInterior : Causality::PastInterior;

}

const char* name(Causality c) {
    switch (c) {
        case Causality::FutureCone:     return "FutureCone";
        case Causality::PastCone:       return "PastCone";
        case Causality::FutureInterior: return "FutureInterior";
        case Causality::PastInterior:   return "PastInterior";
        case Causality::Outside:        return "Outside";
        default:                        return "Unknown";
    }
}

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

int main() {

    double v = 0.0; // observer velocity
    std::vector<Event> events;

    Event origin { 0, 0, 0, 0 };
    Event cases[] = {
        { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 1, 5, 0, 0 },
        {-1, 0, 0, 0 }, {-1, 1, 0, 0 }, { 0, 0, 0, 0 },
    };

    for (const auto& e : cases)
        printf("t=%+.0f x=%+.0f  ->  %s\n", e.t, e.x, name(classify(origin, e)));

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