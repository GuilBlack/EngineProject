#include "ScriptingSystem.h"
#include "src/ecs/ComponentManager.h"
#include "src/ecs/components/Script.h"

namespace DXH
{
void ScriptingSystem::Update(const Timer& gt)
{
    // Copy all arrays to avoid iterator invalidation
    std::vector<Script*> ScriptsToStart = s_ScriptsToStart;
    std::vector<Script*> Scripts = s_Scripts;


    for (Script* pScript : ScriptsToStart)
    {
        pScript->Start();
        s_ScriptsToStart.erase(std::find(s_ScriptsToStart.begin(), s_ScriptsToStart.end(), pScript));
    }

    for (Script* pScript : Scripts)
    {
        pScript->Update(gt);
    }

    for (Script* pScript : Scripts)
    {
        pScript->LateUpdate();
    }
}
}
