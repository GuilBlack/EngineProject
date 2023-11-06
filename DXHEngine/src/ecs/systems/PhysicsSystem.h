#pragma once
#include "src/ecs/System.h" // For the System base class

namespace DXH
{
class PhysicsSystem : public System
{
public:
    PhysicsSystem() = default;
    ~PhysicsSystem() = default;

    void Update(const Timer& gt) override;

private:

    void ResolveCollisions(const Timer& gt);
    void UpdateRigidBodies(const Timer& gt);
};
}
