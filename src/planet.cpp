#include "planet.h"
#include <raymath.h>
#include <cmath>
#include <vector>

Planet::Planet(float radius, float voxelSize) 
    : radius(radius), voxelSize(voxelSize)
{
    voxelColor = BLUE;
    Generate();
}

bool Planet::IsInsideSphere(float x, float y, float z)
{
    // Use squared distance to avoid square root
    float distSquared = x*x + y*y + z*z;
    float radiusSquared = radius * radius;
    
    // Add a small epsilon to avoid gaps
    return distSquared <= radiusSquared + 0.1f;
}

void Planet::Generate()
{
    voxels.clear();
    
    // Calculate bounds based on radius
    float bound = radius + voxelSize;
    float step = voxelSize;
    
    // Generate sphere using smaller voxels
    for (float x = -bound; x <= bound; x += step)
    {
        for (float y = -bound; y <= bound; y += step)
        {
            for (float z = -bound; z <= bound; z += step)
            {
                if (IsInsideSphere(x, y, z))
                {
                    voxels.push_back({x, y, z});
                }
            }
        }
    }
}
