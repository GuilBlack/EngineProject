#include "ScriptingSystem.h"
#include "src/ecs/ComponentManager.h"
#include "src/ecs/components/Script.h"

namespace DXH
{
void ScriptingSystem::Update(const Timer& gt)
{
    while (!s_ScriptsToDestroy.empty())
    {
        Script* s = s_ScriptsToDestroy.front();
        s_ScriptsToDestroy.pop();
        s_Scripts.erase(std::remove(s_Scripts.begin(), s_Scripts.end(), s), s_Scripts.end());
    }

    while (!s_ScriptsToStart.empty())
    {
        Script* s = s_ScriptsToStart.front();
        s_ScriptsToStart.pop();
        s->Start();
        s_Scripts.push_back(s);
    }

    for (auto s : s_Scripts)
    {
        s->Update(gt);
    }

    for (auto s : s_Scripts)
    {
        s->LateUpdate(gt);
    }
}
}
