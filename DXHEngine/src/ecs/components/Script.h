#pragma once
#include "src/ecs/systems/ScriptingSystem.h" // The system that will update the script
#include "src/ecs/Component.h" // Base class: Component

namespace DXH
{
class Timer;

/// <summary>
/// Base class for all scripts.
/// </summary>
class Script : private Component
{
public:
	Script() = default;
	~Script() = default;

	// Awake is called when the script is added to a game object.
	virtual void Awake() {}
	// Start is called before the first frame update.
	virtual void Start() {}
	// Update is called once per frame.
	virtual void Update(const Timer& gt) {}
	// LateUpdate is called after all Update functions have been called.
	virtual void LateUpdate() {}
	// OnDestroy is called when the script is removed from a game object.
	virtual void OnDestroy() {}

	const GameObject* pGameObject = Component::pGameObject;

private:
	void OnAssign() override
	{
		Awake();
		ScriptingSystem::Register(this); // This will call Start() before the next frame
	}
	void OnDetach() override
	{
		OnDestroy();
		ScriptingSystem::Unregister(this);
	}
};
}