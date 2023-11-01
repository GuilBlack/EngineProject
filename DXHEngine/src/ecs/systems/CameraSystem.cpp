#include "CameraSystem.h"
#include "../ComponentManager.h"
#include "../components/Camera.h"
#include "../components/Transform.h"


void DXH::CameraSystem::Update(const Timer& gt)
{
	auto& cameraMap = ComponentManager<Camera>::GetInstance().GetUsedComponentsMap();
	auto& transformMap = ComponentManager<Transform>::GetInstance().GetUsedComponentsMap();

	for (auto& pair : cameraMap)
	{
		if (!pair.second->IsPrimary)
			continue;

		Vector3 forward = Vector3::Forward;
		auto go = pair.first;
		auto transform = transformMap.at(go);
		
	}
}
