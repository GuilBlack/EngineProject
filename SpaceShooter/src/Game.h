#pragma once
#include <DXHInclude.h>

class Game
{
public:
	inline static Game& GetInstance()
	{
		static Game instance;
		return instance;
	}

	/// <summary>
	/// Call this function to start the engine.
	/// This function will not return until the engine is shut down.
	/// Make sure to call it after initializing all the systems.
	/// </summary>
	void StartEngine();

private:
	Game();
	~Game();

	/// <summary>
	/// Called every frame from the engine.
	/// </summary>
	/// <param name="gt">Game timer, contains variables such as DeltaTime or TotalTime.</param>
	void Update(const DXH::Timer& gt);
};
