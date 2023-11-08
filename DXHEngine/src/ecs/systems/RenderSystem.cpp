#include "RenderSystem.h"
#include "../ComponentManager.h"
#include "src/renderer/Renderer.h"
#include "../components/Render.h"
#include "src/ecs/GameObject.h"
#include "../components/Camera.h"
#include "../GameObject.h"
#include "src/core/Window.h"
#include "src/renderer/Geometry.h"

namespace DXH
{

void DXH::RenderSystem::Update(const Timer& gt)
{
    using namespace DirectX;
    auto& map = ComponentManager<Mesh>::GetInstance().GetUsedComponentsMap();
    auto& cameraMap = ComponentManager<Camera>::GetInstance().GetUsedComponentsMap();
    Frustum camFrustum;
    bool camInMap = false;
    for (auto& pair : cameraMap)
    {
        if (pair.second.IsPrimary)
        {
            camFrustum = Frustum::CreateFromCamera(
                pair.second,
                (float)Window::GetInstance().GetWidth() / Window::GetInstance().GetHeight()
            );
            Renderer::GetInstance().BeginFrame(pair.second, gt);
            camInMap = true;
            break;
        }
    }

    if (!camInMap)
    {
        XMVECTOR pos = Vector3(0.f, 0.f, -10.f).Load();
        XMVECTOR target = XMVectorSet(0.f, 0.f, 0.f, 1.f);
        XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 1.f);
        XMMATRIX view = XMMatrixLookAtLH(pos, target, up);

        Camera defaultCam;
        defaultCam.View = view;
        defaultCam.Target = target;
        XMMATRIX proj = XMMatrixPerspectiveFovLH(
            XMConvertToRadians(65.f),
            (float)Window::GetInstance().GetWidth() / Window::GetInstance().GetHeight(),
            0.001f,
            1000.f
        );
        defaultCam.Proj = proj;
        defaultCam.pGameObject = GameObject::Create();
        Renderer::GetInstance().BeginFrame(defaultCam, gt);
        defaultCam.pGameObject->Destroy();
    }

    for (auto& pair : map)
    {
        auto go = pair.first;
        auto& mesh = pair.second;

        if (mesh.Geo == nullptr || mesh.Mat == nullptr)
            continue;

        if (mesh.Geo->BoundingSphere.IsOnFrustum(camFrustum, *go))
        {
            Renderer::GetInstance().Draw(mesh, *go);
        }
    }

    auto& numMap = ComponentManager<NumberUI>::GetInstance().GetUsedComponentsMap();
    for (auto& pair : numMap)
    {
        auto go = pair.first;
        auto& num = pair.second;

        if (num.Geo == nullptr)
            continue;

        Renderer::GetInstance().DrawNumber(num, *go);
    }

    Renderer::GetInstance().EndFrame();
}
}
