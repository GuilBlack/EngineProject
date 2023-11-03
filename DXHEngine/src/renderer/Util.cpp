#include "Util.h"
#include "src/ecs/components/Transform.h"
#include "src/ecs/components/Camera.h"

namespace DXH
{
bool SphereBoundingVolume::IsOnOrForwardPlane(const Plane& plane) const
{
    return plane.GetSignedDistanceToPlane(Center) > -Radius;
}

bool SphereBoundingVolume::IsOnFrustum(const Frustum& camFrustum, const Transform& transform) const
{
    const Vector3 scale = transform.Scale;

    const float maxScale = MAX(MAX(scale.x, scale.y), scale.z);


    const Vector3 worldSpaceCenter = DirectX::XMVector3Transform(Center.Load(), transform.GetModelMatrix());

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
Frustum Frustum::CreateFromCamera(const Camera& cam, const Transform& camTransform, float aspect)
{
    using namespace DirectX;
    Frustum frustum;

    const float halfVSide = cam.FarPlan * tanf(XMConvertToRadians(cam.FieldOfView) * .5f);
    const float halfHSide = halfVSide * aspect;

    Matrix camRotation = camTransform.GetRotationMatrix();
    XMVECTOR camRight = XMVectorSet(camRotation._11, camRotation._12, camRotation._13, 1.f);
    XMVECTOR camUp = XMVectorSet(camRotation._21, camRotation._22, camRotation._23, 1.f);
    XMVECTOR camForward = cam.Forward.Normalize();
    XMVECTOR frontMultFar = XMVectorScale(camForward, cam.FarPlan);
    XMVECTOR camPos = camTransform.Position.Load();

    frustum.NearFace = 
    { 
        camPos + XMVectorScale(camForward, cam.NearPlan),
        camForward
    };
    frustum.FarFace =
    { 
        camPos + frontMultFar,
        XMVectorNegate(camForward)
    };
    frustum.RightFace =
    {
        camPos,
        XMVector3Cross(frontMultFar + camRight * halfHSide, camUp)
    };
    frustum.LeftFace = 
    {
        camPos,
        XMVector3Cross(camUp,frontMultFar - camRight * halfHSide)
    };
    frustum.TopFace =
    {
        camPos,
        XMVector3Cross(camRight, frontMultFar + camUp * halfVSide)
    };
    frustum.BottomFace =
    {
        camPos,
        XMVector3Cross(frontMultFar - camUp * halfVSide, camRight)
    };

    return frustum;
}
}
