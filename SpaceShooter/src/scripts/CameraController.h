#pragma once
#include <DXHScripting.h>

class CameraController : public DXH::Script
{
public:
	void Start() override;
	void Update(const DXH::Timer& gt) override;
private:
	const float m_MouseSensitivity = 0.5f;

	DXH::Transform* m_camTransform = nullptr;
	DXH::Vector3 m_rotation = DXH::Vector3(0.f, 0.f, 0.f);
	DXH::RigidBody* m_SpaceshipRigibody;
private:
	void UpdateCameraPosition();
};
