#include "RenderSystem.h"
#include "../ComponentManager.h"
#include "src/renderer/Renderer.h"
#include "../components/Render.h"
#include "../components/Transform.h"
#include "../components/Camera.h"
#include "../GameObject.h"
#include "src/core/Window.h"

namespace DXH
{
void DXH::RenderSystem::Update(const Timer& gt)
{
    auto& map = ComponentManager<Mesh>::GetInstance().GetUsedComponentsMap();
    auto& transformMap = ComponentManager<Transform>::GetInstance().GetUsedComponentsMap();
    Renderer::GetInstance().BeginFrame();
    for (auto& pair : map)
    {
        Renderer::GetInstance().Draw(pair.second, transformMap.at(pair.first));
    }
    Renderer::GetInstance().EndFrame();
}

void DXH::RenderSystem::Draw(Mesh * mesh, Transform transform)
{
}
