#include "System.h"

// Include all systems here
#include "systems/ScriptingSystem.h"
#include "systems/GameObjectCollector.h"
#include "systems/PhysicsSystem.h"
#include "systems/CameraSystem.h"
#include "systems/RenderSystem.h"
#include "systems/ParticleSystem.h"

namespace DXH
{
void System::UpdateAll(const Timer& gt)
{
    // Systems creation
    static ScriptingSystem scriptingSystem;
    static PhysicsSystem physicsSystem;
    static CameraSystem cameraSystem;
    static RenderSystem renderSystem;
    static ParticleSystem particleSystem;
    static GameObjectCollector& gameObjectCollector = GameObjectCollector::GetInstance();

    // Systems update
    scriptingSystem.Update(gt);
    physicsSystem.Update(gt);
    cameraSystem.Update(gt);
    particleSystem.Update(gt);
    renderSystem.Update(gt);
    gameObjectCollector.Update(gt);
}
}
