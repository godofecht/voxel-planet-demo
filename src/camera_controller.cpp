#include "camera_controller.h"
#include <cmath>

CameraController::CameraController()
{
    moveSpeed = 10.0f;
    lookSpeed = 2.0f;
    
    // Initialize vectors
    position = {50.0f, 50.0f, 50.0f};
    rotation = {0.0f, 0.0f, 0.0f};

    // Setup camera
    camera.position = position;
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void CameraController::Update(float deltaTime)
{
    // Mouse look
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 mouseDelta = GetMouseDelta();
        rotation.x -= mouseDelta.y * lookSpeed * deltaTime;
        rotation.y -= mouseDelta.x * lookSpeed * deltaTime;

        // Clamp pitch rotation
        if (rotation.x > 89.0f) rotation.x = 89.0f;
        if (rotation.x < -89.0f) rotation.x = -89.0f;

        // Convert rotation to direction
        float pitch = rotation.x * DEG2RAD;
        float yaw = rotation.y * DEG2RAD;
        
        Vector3 direction = {
            cosf(pitch) * cosf(yaw),
            sinf(pitch),
            cosf(pitch) * sinf(yaw)
        };

        camera.target = Vector3Add(position, direction);
    }

    // Movement
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

    if (IsKeyDown(KEY_W)) position = Vector3Add(position, Vector3Scale(forward, moveSpeed * deltaTime));
    if (IsKeyDown(KEY_S)) position = Vector3Subtract(position, Vector3Scale(forward, moveSpeed * deltaTime));
    if (IsKeyDown(KEY_D)) position = Vector3Add(position, Vector3Scale(right, moveSpeed * deltaTime));
    if (IsKeyDown(KEY_A)) position = Vector3Subtract(position, Vector3Scale(right, moveSpeed * deltaTime));
    if (IsKeyDown(KEY_SPACE)) position.y += moveSpeed * deltaTime;
    if (IsKeyDown(KEY_LEFT_SHIFT)) position.y -= moveSpeed * deltaTime;

    camera.position = position;
}

Camera3D& CameraController::GetCamera()
{
    return camera;
}
