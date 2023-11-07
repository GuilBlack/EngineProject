#pragma once
#include "src/ecs/System.h" // For the System base class

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
};
}
