#pragma once
#include "../System.h"
#include "src/DXHMaths.h" // For the Vector3 struct

namespace DXH
{
struct Mesh;
struct Transform;

class RenderSystem : public System
{
public:
    RenderSystem() {}
    ~RenderSystem() {}

    virtual void Update(const Timer& gt) override;
    void Draw(Mesh* mesh, Transform transform);
};
}

