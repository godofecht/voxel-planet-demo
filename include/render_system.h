#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <raylib.h>
#include <vector>
#include <memory>
#include "planet.h"
#include "frustum.h"

class RenderSystem 
{
public:
    static RenderSystem& GetInstance();
    
    void BeginFrame(const Camera3D& camera);
    void EndFrame();
    void DrawPlanet(const Planet& planet);

private:
    RenderSystem() = default;
    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;

    void DrawVoxel(const Vector3& position, float size, Color color);
    const Camera3D* activeCamera{nullptr};
    Frustum frustum;
};

#endif // RENDER_SYSTEM_H
