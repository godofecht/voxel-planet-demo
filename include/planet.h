#ifndef PLANET_H
#define PLANET_H

#include <raylib.h>
#include <vector>

class Planet 
{
public:
    Planet(float radius = 32.0f, float voxelSize = 0.5f);
    void Generate();

    // Getters for the render system
    const std::vector<Vector3>& GetVoxels() const { return voxels; }
    float GetVoxelSize() const { return voxelSize; }
    Color GetVoxelColor() const { return voxelColor; }

private:
    bool IsInsideSphere(float x, float y, float z);
    
    float radius;
    float voxelSize;
    std::vector<Vector3> voxels;
    Color voxelColor;
};

#endif // PLANET_H
