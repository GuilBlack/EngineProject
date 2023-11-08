#pragma once
#include "src/ecs/System.h" // Base class: System
#include <queue>

namespace DXH
{
class Script;

class ScriptingSystem : public System
{
    friend class System;
    ScriptingSystem() = default;
    ~ScriptingSystem() = default;
public:

    // Register a script component to be updated
    static inline void Register(Script* pScript) { s_ScriptsToStart.push(pScript); }
    // Unregister a script component
    static inline void Unregister(Script* pScript) { s_ScriptsToDestroy.push(pScript); }

    void Update(const Timer& gt) override;

private:
    inline static std::vector<Script*> s_Scripts;
    inline static std::queue<Script*> s_ScriptsToStart;
    inline static std::queue<Script*> s_ScriptsToDestroy;
};
}
