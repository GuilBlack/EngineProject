#include "System.h"

// Include all systems here
#include "systems/PhysicsSystem.h"

namespace DXH
{
void System::UpdateAll(const Timer& gt)
{
	// Systems creation
	static PhysicsSystem physicsSystem;

	// Systems update
	physicsSystem.Update(gt);
}
}
