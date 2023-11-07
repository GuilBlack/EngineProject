#pragma once
#include "src/ecs/System.h" // Base class: System
#include <vector>

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
    static inline void Register(Script* pScript) 
    { 
        s_Scripts.push_back(pScript); 
        s_ScriptsToStart.push_back(pScript); 
    }
    // Unregister a script component
    static inline void Unregister(Script* pScript) { s_Scripts.erase(std::find(s_Scripts.begin(), s_Scripts.end(), pScript)); }

    void Update(const Timer& gt) override;

private:
    inline static std::vector<Script*> s_Scripts;
    inline static std::vector<Script*> s_ScriptsToStart;
};
}
