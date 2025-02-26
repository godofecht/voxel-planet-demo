#include "frustum.h"
#include <cmath>

void Frustum::Update(const Camera3D& camera)
{
    const float halfVFov = camera.fovy * 0.5f * DEG2RAD;
    const float halfHFov = halfVFov * (16.0f/9.0f); // Assuming 16:9 aspect ratio
    const float near = 0.1f;
    const float far = 1000.0f;
    
    Vector3 cameraPos = camera.position;
    Vector3 cameraTarget = camera.target;
    Vector3 forward = Vector3Normalize(Vector3Subtract(cameraTarget, cameraPos));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
    Vector3 up = Vector3CrossProduct(right, forward);
    
    // Near plane
    planes[0] = CreatePlane(forward, Vector3Add(cameraPos, Vector3Scale(forward, near)));
    
    // Far plane
    planes[1] = CreatePlane(Vector3Scale(forward, -1.0f), Vector3Add(cameraPos, Vector3Scale(forward, far)));
    
    // Left plane
    Vector3 leftNormal = Vector3Normalize(Vector3CrossProduct(up, Vector3RotateByAxisAngle(forward, up, -halfHFov)));
    planes[2] = CreatePlane(leftNormal, cameraPos);
    
    // Right plane
    Vector3 rightNormal = Vector3Normalize(Vector3CrossProduct(Vector3RotateByAxisAngle(forward, up, halfHFov), up));
    planes[3] = CreatePlane(rightNormal, cameraPos);
    
    // Top plane
    Vector3 topNormal = Vector3Normalize(Vector3CrossProduct(right, Vector3RotateByAxisAngle(forward, right, -halfVFov)));
    planes[4] = CreatePlane(topNormal, cameraPos);
    
    // Bottom plane
    Vector3 bottomNormal = Vector3Normalize(Vector3CrossProduct(Vector3RotateByAxisAngle(forward, right, halfVFov), right));
    planes[5] = CreatePlane(bottomNormal, cameraPos);
}

Vector4 Frustum::CreatePlane(const Vector3& normal, const Vector3& point)
{
    Vector3 normalizedNormal = Vector3Normalize(normal);
    return {
        normalizedNormal.x,
        normalizedNormal.y,
        normalizedNormal.z,
        -Vector3DotProduct(normalizedNormal, point)
    };
}

float Frustum::DistanceToPlane(const Vector4& plane, const Vector3& point)
{
    return plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w;
}

bool Frustum::IsPointInFrustum(const Vector3& point, float radius) const
{
    for (const auto& plane : planes)
    {
        if (DistanceToPlane(plane, point) < -radius)
        {
            return false;
        }
    }
    return true;
}
