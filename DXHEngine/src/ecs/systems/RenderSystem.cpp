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
    using namespace DirectX;
    auto& map = ComponentManager<Mesh>::GetInstance().GetUsedComponentsMap();
    auto& transformMap = ComponentManager<Transform>::GetInstance().GetUsedComponentsMap();
    auto& cameraMap = ComponentManager<Camera>::GetInstance().GetUsedComponentsMap();
    Camera* cam = nullptr;

    for (auto& pair : cameraMap)
    {
        if (pair.second->IsPrimary)
        {
            cam = pair.second;
            break;
        }
    }

    Camera defaultCam;
    if (cam == nullptr)
    {
        XMVECTOR pos = Vector3(0.f,0.f,-10.f).Load();
        XMVECTOR target = XMVectorSet(0.f, 0.f, 0.f, 1.f);
        XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 1.f);
        XMMATRIX view = XMMatrixLookAtLH(pos, target, up);

        defaultCam.View = view;
        defaultCam.Target = target;
        XMMATRIX proj = XMMatrixPerspectiveFovLH(
            XMConvertToRadians(65.f),
            (float)Window::GetInstance().GetWidth() / Window::GetInstance().GetHeight(),
            0.001f,
            1000.f
        );
        defaultCam.Proj = proj;
        Renderer::GetInstance().BeginFrame(defaultCam);
    }
    else
    {
        Renderer::GetInstance().BeginFrame(*cam);
    }

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
}
