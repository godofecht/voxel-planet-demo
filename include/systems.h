#pragma once

#include <flecs.h>
#include "components.h"
#include "render_system.h"
#include "voxel_mesher.h"
#include <memory>

struct Systems {
    static std::unique_ptr<VoxelMesher> mesher;

    // Initialize systems
    static void Initialize() {
        mesher = std::make_unique<VoxelMesher>();
        mesher->Initialize();
    }

    // Planet mesh update system
    static void PlanetMeshSystem(flecs::iter& it,
                               Components::Planet* planet) {
        for (auto i : it) {
            if (planet[i].needsRemesh) {
                // Generate new mesh
                planet[i].mesh = mesher->GenerateMesh(planet[i].voxelPositions);
                planet[i].needsRemesh = false;
            }
        }
    }

    // Render system that processes all renderable entities
    static void RenderSystem(const Components::Transform& transform,
                           const Components::Planet& planet,
                           const Components::Renderable& renderable) {
        if (!renderable.visible) return;
        
        // Draw the planet mesh
        DrawModel(planet.mesh, transform.position, 1.0f, WHITE);
    }

    // Register systems with Flecs
    static void RegisterSystems(flecs::world& ecs) {
        Initialize();

        // Planet mesh generation system
        ecs.system<Components::Planet>()
            .kind(flecs::PreUpdate)
            .each(PlanetMeshSystem);

        // Rendering system
        ecs.system<Components::Transform, Components::Planet, Components::Renderable>()
            .each(RenderSystem);
    }
};
