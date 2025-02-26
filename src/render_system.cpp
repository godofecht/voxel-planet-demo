#include "render_system.h"
#include <raymath.h>

RenderSystem& RenderSystem::GetInstance()
{
    static RenderSystem instance;
    return instance;
}

void RenderSystem::BeginFrame(const Camera3D& camera)
{
    activeCamera = &camera;
    frustum.Update(camera);
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(camera);
    DrawGrid(100, 10.0f);
}

void RenderSystem::EndFrame()
{
    EndMode3D();
    DrawFPS(10, 10);
    DrawText("Right Click + Mouse to look around", 10, 30, 20, GREEN);
    DrawText("WASD to move, Space/Shift for Up/Down", 10, 50, 20, GREEN);
    EndDrawing();
    activeCamera = nullptr;
}

void RenderSystem::DrawVoxel(const Vector3& position, float size, Color color)
{
    // Only draw if voxel is in view frustum
    if (frustum.IsPointInFrustum(position, size * 0.87f))
    {
        DrawCube(position, size, size, size, color);
    }
}

void RenderSystem::DrawPlanet(const Planet& planet)
{
    const auto& voxels = planet.GetVoxels();
    const float voxelSize = planet.GetVoxelSize();
    const Color voxelColor = planet.GetVoxelColor();

    for (const auto& voxel : voxels)
    {
        DrawVoxel(voxel, voxelSize, voxelColor);
    }
}
