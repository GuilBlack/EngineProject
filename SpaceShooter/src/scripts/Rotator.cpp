#include "Rotator.h"

void Rotator::Update(const DXH::Timer& gt)
{
    pGameObject->Get<DXH::Transform>().Rotation.y += gt.DeltaTime() * 20; // 1 rotation / sec
}