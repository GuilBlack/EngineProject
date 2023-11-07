#pragma once
#include <DXHScripting.h>

class Score : public DXH::Script
{
public:
    void Start() override {};
    void Update(const DXH::Timer& gt) override;

private:
    uint32_t m_Score = 0;
    float m_DeltaTime = 0;
};
