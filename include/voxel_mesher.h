#pragma once

#include <openvdb/openvdb.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <raylib.h>
#include <vector>
#include <memory>

class VoxelMesher {
public:
    VoxelMesher();
    ~VoxelMesher();

    // Initialize the mesher
    void Initialize();

    // Generate a mesh from voxel data
    Model GenerateMesh(const std::vector<Vector3>& voxels, float isoValue = 0.5f);

    // Update the density field
    void UpdateDensityField(const Vector3& position, float value, float radius);

private:
    openvdb::FloatGrid::Ptr densityGrid;
    float voxelSize;
    
    // Convert raylib Vector3 to OpenVDB Vec3s
    openvdb::Vec3s ToVDBVec3(const Vector3& v) {
        return openvdb::Vec3s(v.x, v.y, v.z);
    }
    
    // Convert OpenVDB Vec3s to raylib Vector3
    Vector3 ToRaylibVec3(const openvdb::Vec3s& v) {
        return Vector3{v.x(), v.y(), v.z()};
    }
};
