#include "Script.h"
#include "src/ecs/GameObject.h"
#include <functional>

namespace DXH
{
void Script::OnAssign()
{
    pGameObject->m_Scripts.push_back(this);
    Awake();
    ScriptingSystem::Register(this); // This will call Start() before the next frame
}

void Script::OnDetach()
{
    OnDestroy();
    pGameObject->m_Scripts.erase(std::find(pGameObject->m_Scripts.begin(), pGameObject->m_Scripts.end(), this));
    ScriptingSystem::Unregister(this);
}
}
