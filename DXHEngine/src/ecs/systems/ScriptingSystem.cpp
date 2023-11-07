#include "ScriptingSystem.h"
#include "src/ecs/ComponentManager.h"
#include "src/ecs/components/Script.h"

namespace DXH
{
void ScriptingSystem::Update(const Timer& gt)
{
    while (s_ScriptsToStart.size() > 0)
    {
        s_ScriptsToStart.back()->Start();
        s_ScriptsToStart.pop_back();
    }

    for (Script* pScript : s_Scripts)
    {
        pScript->Update(gt);
    }

    for (Script* pScript : s_Scripts)
    {
        pScript->LateUpdate();
    }
}
}
