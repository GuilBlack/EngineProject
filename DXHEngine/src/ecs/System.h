#pragma once

namespace DXH
{
class Timer;

/// <summary>
/// Base class for any game system.
/// A system must initialize itself in its constructor.
/// All systems will be created before the first update.
/// </summary>
class System
{
public:
	/// <summary>
	/// Updates the system. This is called once per frame.
	/// </summary>
	/// <param name="gt">Game timer, contains informations like delta time.</param>
	virtual void Update(const Timer& gt) = 0;

	/// <summary>
	/// Updates all systems.
	/// </summary>
	static void UpdateAll(const Timer& gt);
};
}
