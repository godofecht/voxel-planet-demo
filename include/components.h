#pragma once

#include <raylib.h>
#include <flecs.h>
#include <vector>
#include "voxel_mesher.h"

// Components for our ECS system
struct Components {
    // Transform component for position and rotation
    struct Transform {
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
    };

    // Voxel component with density field
    struct Voxel {
        Color color;
        bool active;
        float density;
        float radius;
    };

    // Planet component with meshing data
    struct Planet {
        float radius;
        float voxelScale;
        Model mesh;
        std::vector<Vector3> voxelPositions;
        bool needsRemesh;
    };

    // Renderable component
    struct Renderable {
        bool visible;
    };
};

// Register components with Flecs
inline void RegisterComponents(flecs::world& ecs) {
    ecs.component<Components::Transform>();
    ecs.component<Components::Voxel>();
    ecs.component<Components::Planet>();
    ecs.component<Components::Renderable>();
}
