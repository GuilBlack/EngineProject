#pragma once
#include "src/DXHScripting.h"

class Rotator : public DXH::Script
{
public:
    void Update(const DXH::Timer& gt) override;
};

