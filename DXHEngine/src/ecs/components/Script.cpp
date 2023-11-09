#include "Script.h"
#include "src/ecs/GameObject.h"
#include <functional>

namespace DXH
{
void Script::OnAssign()
{
    pGameObject->m_OnCollisions += std::bind(&Script::OnCollision, this, std::placeholders::_1);
    Awake();
    ScriptingSystem::Register(this); // This will call Start() before the next frame
}

void Script::OnDetach()
{
    OnDestroy();
    ScriptingSystem::Unregister(this);
}
}
