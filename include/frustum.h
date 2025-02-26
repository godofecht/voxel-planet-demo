#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <raylib.h>
#include <raymath.h>
#include <array>

class Frustum 
{
public:
    void Update(const Camera3D& camera);
    bool IsPointInFrustum(const Vector3& point, float radius = 0.0f) const;

private:
    // Frustum planes in order: Near, Far, Left, Right, Top, Bottom
    std::array<Vector4, 6> planes;
    
    static Vector4 CreatePlane(const Vector3& normal, const Vector3& point);
    static float DistanceToPlane(const Vector4& plane, const Vector3& point);
};

#endif // FRUSTUM_H
