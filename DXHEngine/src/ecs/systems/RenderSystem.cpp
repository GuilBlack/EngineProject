#include "RenderSystem.h"
#include "../../renderer/Renderer.h"
#include "../components/Transform.h"

void DXH::RenderSystem::Update(const Timer& gt)
{
	Renderer::GetInstance().BeginFrame();

	Renderer::GetInstance().EndFrame();
}

void DXH::RenderSystem::Draw(Mesh * mesh, Transform transform)
{}
