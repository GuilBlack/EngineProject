#include "System.h"

// Include all systems here
#include "systems/PhysicsSystem.h"
#include "systems/RenderSystem.h"

namespace DXH
{
void System::UpdateAll(const Timer& gt)
{
	// Systems creation
	static PhysicsSystem physicsSystem;
	static RenderSystem renderSystem;

	// Systems update
	physicsSystem.Update(gt);
	renderSystem.Update(gt);
}
}
