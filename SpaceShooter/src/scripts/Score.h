#pragma once
#include <DXHScripting.h>

class Score : public DXH::Script
{
public:
    void Start() override {};
    void Update(const DXH::Timer& gt) override;
    void SetSpaceShip(DXH::GameObject* ss) { m_SpaceShip = ss; }

private:
    uint32_t m_Score = 0;
    float m_DeltaTime = 0;
    DXH::GameObject* m_SpaceShip = nullptr;
};
