#include "System.h"

// Include all systems here
#include "systems/PhysicsSystem.h"
#include "systems/CameraSystem.h"
#include "systems/RenderSystem.h"

namespace DXH
{
void System::UpdateAll(const Timer& gt)
{
	// Systems creation
	static PhysicsSystem physicsSystem;
	static RenderSystem renderSystem;
	static CameraSystem cameraSystem;

	// Systems update
	cameraSystem.Update(gt);
	physicsSystem.Update(gt);
	renderSystem.Update(gt);
}
}
