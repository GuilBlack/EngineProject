#include "CameraSystem.h"
#include "../ComponentManager.h"
#include "src/ecs/GameObject.h"
#include "../components/Camera.h"
#include "../../maths/Quaternion.h"
#include "../../maths/Matrix.h"
#include "../../core/Window.h"

namespace DXH
{
void CameraSystem::Update(const Timer& gt)
{
    using namespace DirectX;

    auto& cameraMap = ComponentManager<Camera>::GetInstance().GetUsedComponentsMap();

    for (auto& pair : cameraMap)
    {
        if (!pair.second.IsPrimary)
            continue;

        auto go = pair.first;
        auto& cam = pair.second;
        Matrix rotationMatrix = go->GetRotationMatrix();
        Vector3 forward = Vector3::Forward;
        cam.Forward = XMVector3Transform(forward.Load(), rotationMatrix.Load());
        cam.Target = cam.Forward.Load() + go->Position().Load();
        cam.View = XMMatrixLookAtLH(go->Position().Load(), cam.Target.Load(), Vector3::Up.Load());
        cam.Proj = XMMatrixPerspectiveFovLH(
            XMConvertToRadians(cam.FieldOfView),
            (float)Window::GetInstance().GetWidth() / Window::GetInstance().GetHeight(),
            cam.NearPlan,
            cam.FarPlan
        );
    }
}
}
