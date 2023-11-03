#include "Rotator.h"

void Rotator::Update(const DXH::Timer& gt)
{
    pGameObject->Get<DXH::Transform>().Rotation.SetRotationFromAngles(0.f, gt.TotalTime() * 20.f, 0.f);
}