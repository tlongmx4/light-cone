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

Vector3 toScreen(const Event& e) {
    return { (float)e.x, (float)e.t, (float)e.z };
}

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

struct Label { Vector3 pos; const char* text; Color color; };

const Label labels[] = {
    { {  0,   0, 0 }, "HERE-NOW",           WHITE   },
    { {  0,   7, 0 }, "ABSOLUTE FUTURE",    SKYBLUE },
    { {  0,  -7, 0 }, "ABSOLUTE PAST",      ORANGE  },
    { {  7,   7, 0 }, "NULL CONE",          YELLOW  },
    { { 13,   0, 0 }, "ABSOLUTE ELSEWHERE", RED     },
};

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

Color colorFor(Causality c) {
    switch (c) {
        case Causality::FutureCone:
        case Causality::PastCone:       return YELLOW;
        case Causality::FutureInterior: return SKYBLUE;
        case Causality::PastInterior:   return ORANGE;
        case Causality::Outside:        return RED;
        default:                        return GRAY;
    }
}

int main() {

    double v = 0.0; // observer velocity
    std::vector<Event> events;

    Event origin { 0, 0, 0, 0 };
    Event cases[] = {
        { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 1, 5, 0, 0 },
        {-1, 0, 0, 0 }, {-1, 1, 0, 0 }, { 0, 0, 0, 0 },
    };

    events.push_back({ 2.0, 3.0, 0.0, 0.0 });
    events.push_back({ 1.0, 1.0, 0.0, 0.0 });
    events.push_back({ -2.0, 0.5, 0.0, 0.0 });
    events.push_back({ 4.0, 10.0, 0.0, 0.0 });

    for (const auto& e : cases)
        printf("t=%+.0f x=%+.0f  ->  %s\n", e.t, e.x, name(classify(origin, e)));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Light Cone");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = { 20.0f, 15.0f, 20.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 25.0f;
    camera.projection = CAMERA_ORTHOGRAPHIC;

    float radius = 35.0f;
    float theta  = 0.785f;
    float phi    = 1.2f;

    while (!WindowShouldClose()) {

        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            radius -= wheel * radius * 0.1f;
            if (radius < 3.0f) radius = 3.0f;
            if (radius > 100.0f) radius = 100.0f;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();

            theta -= delta.x * 0.005f;
            phi += delta.y * 0.005f;

            if (phi > 3.09f) phi = 3.09f;
            if (phi < 0.5f) phi = 0.5f;
        }

        camera.position.x = camera.target.x + radius * sinf(phi) * sinf(theta);
        camera.position.y = camera.target.y + radius * cosf(phi);
        camera.position.z = camera.target.z + radius * sinf(phi) * cosf(theta);

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);

        for (const auto& e : events)
            DrawSphere(toScreen(e), 0.2f, colorFor(classify(origin, e)));

        DrawGrid(20, 1.0f);

        DrawCubeWires({0, 0, 0}, 20, 20, 20, DARKGRAY);

        DrawLine3D({0,-10,0}, {0,10,0}, GREEN);
        DrawLine3D({-10,0,0}, {10,0,0}, RED);
        DrawLine3D({0,0,-10}, {0,0,10}, BLUE);

        DrawCylinderWiresEx({0,0,0}, {0,10,0}, 0.0f, 10.0f, 24, SKYBLUE);
        DrawCylinderWiresEx({0,0,0}, {0,-10,0}, 0.0f, 10.0f, 24, ORANGE);


        EndMode3D();
        for (const auto& l : labels) {
            Vector2 p = GetWorldToScreen(l.pos, camera);
            DrawText(l.text, p.x, p.y, 18, l.color);
        }

        DrawFPS(10, 10);
        DrawText("Left Click + Drag to Orbit. Scroll to Zoom.", 10, 40, 20, GREEN);

        EndDrawing();

    }

    CloseWindow();
    return 0;
}