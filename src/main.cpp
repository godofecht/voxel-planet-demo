#include <raylib.h>
#include <flecs.h>
#include "planet.h"
#include "camera_controller.h"
#include "render_system.h"
#include "components.h"
#include "systems.h"

int main()
{
    // Initialize window
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Voxel Planet");

    // Initialize ECS world
    flecs::world ecs;

    // Register components and systems
    RegisterComponents(ecs);
    Systems::RegisterSystems(ecs);

    // Create camera controller
    CameraController cameraController;

    // Create planet entity with components
    Components::Planet planetData = {
        32.0f,      // radius
        0.5f,       // voxelScale
        { 0 },      // mesh (initialized empty)
        { },        // voxelPositions (empty vector)
        true        // needsRemesh
    };
    
    Components::Transform planetTransform = {
        {0, 0, 0},    // position
        {0, 0, 0},    // rotation
        {1, 1, 1}     // scale
    };
    
    Components::Renderable planetRenderable = {true};

    auto planet = ecs.entity()
        .set<Components::Planet>(planetData)
        .set<Components::Transform>(planetTransform)
        .set<Components::Renderable>(planetRenderable);

    // Create initial voxels for the planet
    // This is a simplified version - you'll want to implement proper planet generation
    for (int x = -16; x < 16; x++) {
        for (int y = -16; y < 16; y++) {
            for (int z = -16; z < 16; z++) {
                float dist = sqrtf(x*x + y*y + z*z);
                if (dist < 16.0f) {
                    // Add voxel position to planet's voxel list
                    auto& planetComponent = planet.get_mut<Components::Planet>();
                    planetComponent->voxelPositions.push_back(Vector3{
                        (float)x * planetComponent->voxelScale,
                        (float)y * planetComponent->voxelScale,
                        (float)z * planetComponent->voxelScale
                    });
                }
            }
        }
    }

    SetTargetFPS(60);
    DisableCursor();

    // Get render system instance
    auto& renderer = RenderSystem::GetInstance();

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update camera
        cameraController.Update(GetFrameTime());

        // Update ECS world
        ecs.progress(GetFrameTime());

        // Draw frame
        renderer.BeginFrame(cameraController.GetCamera());
        ecs.progress(); // This will trigger our render system
        renderer.EndFrame();
    }

    CloseWindow();
    return 0;
}
