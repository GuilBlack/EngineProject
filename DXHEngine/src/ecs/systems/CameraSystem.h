#pragma once
#include "../System.h"
namespace DXH 
{

struct Transform;
struct Camera;

class CameraSystem : public System 
{
public:
    CameraSystem(){}
    ~CameraSystem(){}

    virtual void Update(const Timer& gt) override;
};
}
