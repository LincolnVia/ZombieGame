#include "raylib.h"
#include <vector>
#include "collision.h"
#include "Player.h"
#include "tiledCube.h"
#include "raymath.h"
int main(void)
{
    InitWindow(1280, 720, "FPS Zombies");
    DisableCursor();

    std::vector<AABB> colliders;
    std::vector<AABB> cubeColliders;

    Player player({0.0f, 5.0f, 0.0f});  // start player a bit above ground

    Texture2D grassTexture = LoadTexture("src/resources/grass.png");
    Texture2D brickTexture = LoadTexture("src/resources/brick.png");
    int tileRepeat = 4;

    MeshWithMaterial floorMesh = CreateTiledCubeWithColliderAndTexture(cubeColliders, {0,0,0}, {50,0.2f,50}, tileRepeat, grassTexture);
    MeshWithMaterial wallMesh = CreateTiledCubeWithColliderAndTexture(cubeColliders, {3,0,10}, {10,4,2},tileRepeat ,brickTexture);
    // Add cube colliders to general colliders list
    colliders.insert(colliders.end(), cubeColliders.begin(), cubeColliders.end());

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        player.Update(dt, colliders);

        Camera3D cam = player.GetCamera();

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(cam);

        DrawMeshAtPosition(floorMesh, {0, 0, 0});
        DrawMeshAtPosition(wallMesh,wallMesh.position);
        // Debug colliders
        for (const auto& box : colliders)
        {
            Vector3 size = Vector3Subtract(box.max, box.min);
            Vector3 center = Vector3Add(box.min, Vector3Scale(size, 0.5f));
            DrawCubeWires(center, size.x, size.y, size.z, BLACK);
        }

        player.Draw();

        EndMode3D();

        DrawText("FPS Zombies Player Test", 10, 10, 20, DARKGRAY);
        DrawFPS(10, 40);

        EndDrawing();
    }

    UnloadTexture(grassTexture);
    CloseWindow();
    return 0;
}
