#include "RenderSystem.h"
#include "../../renderer/Renderer.h"
#include "../components/Render.h"
#include "../components/Transform.h"
#include "../ComponentManager.h"
#include "../GameObject.h"

void DXH::RenderSystem::Update(const Timer& gt)
{
	auto& map = ComponentManager<Mesh>::GetInstance().GetUsedComponentsMap();
	auto& transformMap = ComponentManager<Transform>::GetInstance().GetUsedComponentsMap();
	Renderer::GetInstance().BeginFrame();
	for (auto& pair : map)
	{
		auto go = pair.first;
		auto mesh = pair.second;
		auto transform = transformMap.at(go);
		Renderer::GetInstance().Draw(*mesh, *transform);
	}
	Renderer::GetInstance().EndFrame();
}

void DXH::RenderSystem::Draw(Mesh * mesh, Transform transform)
{}
