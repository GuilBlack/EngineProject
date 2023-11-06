#include "Util.h"
#include "src/ecs/GameObject.h"
#include "src/ecs/components/Camera.h"

namespace DXH
{

bool SphereBoundingVolume::IsOnFrustum(const Frustum& camFrustum, const GameObject& gameObject) const
{
    const Vector3 scale = gameObject.Scale();

    const float maxScale = MAX(MAX(scale.x, scale.y), scale.z);


    const Vector3 worldSpaceCenter = DirectX::XMVector3Transform(Center.Load(), gameObject.GetModelMatrix());

    SphereBoundingVolume worldSphere(worldSpaceCenter, Radius * (maxScale));

    return (
        worldSphere.IsOnOrForwardPlane(camFrustum.FarFace) &&
        worldSphere.IsOnOrForwardPlane(camFrustum.NearFace) &&
        worldSphere.IsOnOrForwardPlane(camFrustum.TopFace) &&
        worldSphere.IsOnOrForwardPlane(camFrustum.BottomFace) &&
        worldSphere.IsOnOrForwardPlane(camFrustum.LeftFace) &&
        worldSphere.IsOnOrForwardPlane(camFrustum.RightFace)
    );
}
Frustum Frustum::CreateFromCamera(const Camera& cam, float aspect)
{
    using namespace DirectX;

    const float halfVSide = cam.FarPlan * tanf(XMConvertToRadians(cam.FieldOfView) * .5f);
    const float halfHSide = halfVSide * aspect;

    Matrix camRotation = cam.pGameObject->GetRotationMatrix();
    XMVECTOR camRight = XMVectorSet(camRotation._11, camRotation._12, camRotation._13, 1.f);
    XMVECTOR camUp = XMVectorSet(camRotation._21, camRotation._22, camRotation._23, 1.f);
    XMVECTOR camForward = cam.Forward.Normalize();
    XMVECTOR frontMultFar = XMVectorScale(camForward, cam.FarPlan);
    XMVECTOR camPos = cam.pGameObject->Position().Load();
    
    Frustum frustum =
    {
        .TopFace =
        {
            camPos,
            XMVector3Cross(camRight, frontMultFar + camUp * halfVSide)
        },
        .BottomFace =
        {
            camPos,
            XMVector3Cross(frontMultFar - camUp * halfVSide, camRight)
        },
        .LeftFace =
        {
            camPos,
            XMVector3Cross(camUp,frontMultFar - camRight * halfHSide)
        },
        .RightFace =
        {
            camPos,
            XMVector3Cross(frontMultFar + camRight * halfHSide, camUp)
        },
        .NearFace =
        {
            camPos + XMVectorScale(camForward, cam.NearPlan),
            camForward
        },
        .FarFace =
        {
            camPos + frontMultFar,
            XMVectorNegate(camForward)
        }
    };

    return frustum;
}

}
