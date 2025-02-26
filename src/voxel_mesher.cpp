#include "voxel_mesher.h"
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/LevelSetSphere.h>

// Define static member
std::unique_ptr<VoxelMesher> Systems::mesher;

VoxelMesher::VoxelMesher() : voxelSize(0.5f) {
    openvdb::initialize();
}

VoxelMesher::~VoxelMesher() {
}

void VoxelMesher::Initialize() {
    // Create a new grid with background value of 0
    densityGrid = openvdb::FloatGrid::create(0.0f);
    densityGrid->setGridClass(openvdb::GRID_LEVEL_SET);
    densityGrid->setTransform(openvdb::math::Transform::createLinearTransform(voxelSize));
}

void VoxelMesher::UpdateDensityField(const Vector3& position, float value, float radius) {
    auto accessor = densityGrid->getAccessor();
    openvdb::Vec3s center = ToVDBVec3(position);
    
    // Create a sphere of influence
    int voxelRadius = static_cast<int>(radius / voxelSize);
    openvdb::Coord min(
        static_cast<int>(center.x() - voxelRadius),
        static_cast<int>(center.y() - voxelRadius),
        static_cast<int>(center.z() - voxelRadius)
    );
    openvdb::Coord max(
        static_cast<int>(center.x() + voxelRadius),
        static_cast<int>(center.y() + voxelRadius),
        static_cast<int>(center.z() + voxelRadius)
    );
    
    // Update density values within the sphere
    for (auto z = min.z(); z <= max.z(); ++z) {
        for (auto y = min.y(); y <= max.y(); ++y) {
            for (auto x = min.x(); x <= max.x(); ++x) {
                openvdb::Coord xyz(x, y, z);
                openvdb::Vec3s pos = densityGrid->indexToWorld(xyz);
                float dist = (pos - center).length();
                
                if (dist <= radius) {
                    float density = value * (1.0f - (dist / radius));
                    accessor.setValue(xyz, accessor.getValue(xyz) + density);
                }
            }
        }
    }
}

Model VoxelMesher::GenerateMesh(const std::vector<Vector3>& voxels, float isoValue) {
    // Clear the grid
    densityGrid->clear();
    
    // Add all voxels to the density field
    for (const auto& voxel : voxels) {
        UpdateDensityField(voxel, 1.0f, voxelSize * 2.0f);
    }
    
    // Convert to mesh using marching cubes
    std::vector<openvdb::Vec3s> points;
    std::vector<openvdb::Vec3I> triangles;
    std::vector<openvdb::Vec4I> quads;
    
    openvdb::tools::volumeToMesh(*densityGrid, points, triangles, quads, isoValue);
    
    // Convert to raylib mesh
    Mesh mesh = { 0 };
    mesh.vertexCount = points.size();
    mesh.triangleCount = triangles.size() + quads.size() * 2; // Each quad becomes 2 triangles
    
    // Allocate vertex data
    mesh.vertices = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.normals = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    
    // Copy vertices
    for (size_t i = 0; i < points.size(); i++) {
        mesh.vertices[i * 3] = points[i].x();
        mesh.vertices[i * 3 + 1] = points[i].y();
        mesh.vertices[i * 3 + 2] = points[i].z();
    }
    
    // Create index buffer
    mesh.indices = (unsigned short*)MemAlloc((triangles.size() * 3 + quads.size() * 6) * sizeof(unsigned short));
    int indexCount = 0;
    
    // Add triangles
    for (const auto& tri : triangles) {
        mesh.indices[indexCount++] = tri.x();
        mesh.indices[indexCount++] = tri.y();
        mesh.indices[indexCount++] = tri.z();
    }
    
    // Add quads (as two triangles each)
    for (const auto& quad : quads) {
        // First triangle
        mesh.indices[indexCount++] = quad.x();
        mesh.indices[indexCount++] = quad.y();
        mesh.indices[indexCount++] = quad.z();
        
        // Second triangle
        mesh.indices[indexCount++] = quad.x();
        mesh.indices[indexCount++] = quad.z();
        mesh.indices[indexCount++] = quad.w();
    }
    
    // Generate normals
    rlGenMeshNormals(&mesh);
    
    // Create model from mesh
    Model model = LoadModelFromMesh(mesh);
    
    return model;
}
