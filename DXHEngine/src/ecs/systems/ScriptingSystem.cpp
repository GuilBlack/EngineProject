#include "ScriptingSystem.h"
#include "src/ecs/ComponentManager.h"
#include "src/ecs/components/Script.h"

namespace DXH
{
void ScriptingSystem::Update(const Timer& gt)
{
	for (Script* pScript : s_ScriptsToStart)
	{
		pScript->Start();
		s_ScriptsToStart.erase(std::find(s_ScriptsToStart.begin(), s_ScriptsToStart.end(), pScript));
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
