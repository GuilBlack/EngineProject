#pragma once
#include "DXHMaths.h"

#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)

namespace DXH
{
struct Transform;
struct Camera;

inline uint32_t GetCBByteSize(uint32_t byteSize)
{
    return (byteSize + int(0xff)) & ~int(0xff);
}

struct Plane
{
    Vector3 Normal = { 0.f, 1.f, 0.f };
    float Distance = 0.f;

    Plane() = default;
    Plane(const Vector3& p, const Vector3& norm)
        : Normal(norm.Normalize()), Distance(Vector3::Dot(Normal, p))
    {}

    float GetSignedDistanceToPlane(const Vector3& p) const
    {
        return Vector3::Dot(Normal, p) - Distance;
    }
};

struct Frustum
{
    Plane TopFace{};
    Plane BottomFace{};

    Plane LeftFace{};
    Plane RightFace{};

    Plane NearFace{};
    Plane FarFace{};

    static Frustum CreateFromCamera(const Camera& cam, const Transform& camTransform, float aspect);
};

struct SphereBoundingVolume
{
    Vector3 Center{ 0.f, 0.f, 0.f };
    float Radius{ 0.f };

    SphereBoundingVolume() = default;
    SphereBoundingVolume(const Vector3 center, float radius)
        : Center(center), Radius(radius)
    {}

    bool IsOnOrForwardPlane(const Plane& plane) const;

    bool IsOnFrustum(const Frustum& camFrustum, const Transform& transform) const;
};

}
