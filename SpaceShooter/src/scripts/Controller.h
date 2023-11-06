#pragma once
#include <DXHScripting.h>

class Controller : public DXH::Script
{
public:
    void Start() override;
    void Update(const DXH::Timer& gt) override;
private:
    const float m_MouseSensitivity = 0.5f;
    //Rotation Axes in Quaternion 
    float m_pitch;
    float m_yaw;
    float m_roll = 0;

    DXH::Transform* m_camTransform = nullptr;
    DXH::Vector3 m_rotation = DXH::Vector3(0.f, 0.f, 0.f);
};
