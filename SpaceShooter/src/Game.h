#pragma once

namespace DXH // Forward declaration
{
class GameObject;
class Timer;
}

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
    DXH::GameObject* m_Camera = nullptr;
private:
    Game() = default; // This should not be used
    ~Game() = default; // Use Init / Destroy instead

    /// <summary>
    /// Initializes the game. Called once before the first engine update.
    /// </summary>
    void Init(const DXH::Timer& gt);

    /// <summary>
    /// Destroys the game. Called once before the engine shuts down.
    ///  </summary>
    void Destroy(const DXH::Timer& gt);

    void LoadAssets();
};
