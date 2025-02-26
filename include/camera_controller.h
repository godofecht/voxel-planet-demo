#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <raylib.h>
#include <raymath.h>

class CameraController 
{
public:
    CameraController();
    void Update(float deltaTime);
    Camera3D& GetCamera();

private:
    Camera3D camera;
    float moveSpeed;
    float lookSpeed;
    Vector3 position;
    Vector3 rotation;
};

#endif // CAMERA_CONTROLLER_H
