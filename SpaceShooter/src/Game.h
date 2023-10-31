#pragma once
#include <DXHInclude.h>

namespace DXH { class Timer; } // Forward declaration
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
	std::vector<DXH::GameObject*> m_GameObjects;

private:
	Game() = default; // This should not be used
	~Game() = default; // Use Init / Destroy instead

	/// <summary>
	/// Initializes the game. Called once before the first engine update.
	/// </summary>
	void Init(const DXH::Timer& gt);

	/// <summary>
	/// Called every frame from the engine.
	/// </summary>
	/// <param name="gt">Game timer, contains variables such as DeltaTime or TotalTime.</param>
	void Update(const DXH::Timer& gt);

	/// <summary>
	/// Destroys the game. Called once before the engine shuts down.
	///  </summary>
	void Destroy(const DXH::Timer& gt);
};
