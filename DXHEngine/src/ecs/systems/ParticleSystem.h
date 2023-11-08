#pragma once
#include "../System.h"

namespace DXH
{
class Timer;

class ParticleSystem : public System
{
public:
    ParticleSystem() {}
    ~ParticleSystem() {}

    virtual void Update(const Timer& gt) override;

};

}
