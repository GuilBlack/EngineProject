#include "GameObject.h"

namespace DXH
{
GameObject::~GameObject()
{
    // Call all the destroy callbacks
    for (auto& callback : m_ReleaseCallbacks)
        callback(this);
}
}
