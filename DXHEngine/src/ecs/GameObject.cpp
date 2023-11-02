#include "GameObject.h"
#include "Components/Transform.h"

namespace DXH
{
GameObject::GameObject()
{
    Add<Transform>();
}

GameObject::~GameObject()
{
    // Call all the destroy callbacks
    for (auto& callback : m_ReleaseCallbacks)
        callback(this);
}
}
