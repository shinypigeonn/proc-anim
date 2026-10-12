#include "raylib.h"
#include "raymath.h"

Color aDarkerGrey = { 20, 28, 32, 255 }; 
Vector2 center = { 910.0f, 600.0f };

struct Head
{
    Vector2 pos;
    float radius;
    Vector2 dir;
    
    void Draw() const
    {
        DrawCircleV(pos, radius, Color{ 95, 175, 150, 255 });
        DrawCircleLinesV(pos, radius, RAYWHITE);
        
        Vector2 side = { -dir.y, dir.x };
        
        // Eyes on the head
        for (int i = 0; i < 2; i++)
        {
            float s = (i == 0) ? -1.0f : 1.0f; // left eye, then right eye
            
            // Eye = center + a bit forward + a bit to the side
            Vector2 forward = Vector2Scale(dir, radius * 0.4f);
            Vector2 sideways = Vector2Scale(side, s * radius * 0.5f);
            Vector2 eye = Vector2Add(pos, Vector2Add(forward, sideways));          
                
            float eyeRadius = radius * 0.22f;
            DrawCircleV(eye, eyeRadius, WHITE);
        }
    }
};

float WrapAngle(float a)
{
    while (a > PI)  a -= 2.0f * PI;
    while (a < -PI) a += 2.0f * PI;
    return a;
}

int main()
{
    InitWindow(1820, 1200, "Procedural Animation");
    SetTargetFPS(60);
    
    Head head = { { 400.0f, 300.0f }, 40.0f, { 1.0f, 0.0f } };
    
    float turnSpeed = 200.0f * DEG2RAD; // max turn speed in radians per second
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        Vector2 toMouse = Vector2Subtract(GetMousePosition(), head.pos);

        if (Vector2Length(toMouse) > 1.0f)
        {
            float current = atan2f(head.dir.y, head.dir.x); // get the heads current angle
            float target  = atan2f(toMouse.y, toMouse.x); // and the target angle (the mouse)
            
            // Finding the difference between them and wrap that into a range of -180 -> 180.
            float diff = WrapAngle(target - current);
            float maxTurn = turnSpeed * dt;
            diff = Clamp(diff, -maxTurn, maxTurn);

            float angle = current + diff;
            head.dir = { cosf(angle), sinf(angle) };

            head.pos = Vector2Add(head.pos, Vector2Scale(toMouse, 0.1f));
        }
        
        BeginDrawing();
        ClearBackground(aDarkerGrey);
        head.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}