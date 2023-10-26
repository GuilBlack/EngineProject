#include "Game.h"
#include "src/ecs/systems/PhysicsSystem.h"

// The entry point of the game
DXH::DXHEngine* DXH::CreateDXHEngine()
{
	DXH::AppProperties props
	{
		.WindowTitle = L"Space Shooter"
	};
	return new Game(props);
}


Game::Game(DXH::AppProperties props) : DXH::DXHEngine(props), _entities(_length)
{
	for (size_t i = 0; i < _length; i++)
	{
		_entities[i].AddComponent<DXH::RigidBody>();
		_entities[i].AddComponent<DXH::SphereCollider>();
	}

	DXH::Timer timer = DXH::Timer(); // Timer for benchmarking

	auto collisions = DXH::PhysicsSystem::ComputeCollisions(_entities);

	VS_DB_OUT_A("Time: " << timer.TotalTime() << std::endl);
}

Game::~Game()
{
}

void Game::Update(const DXH::Timer& gt)
{
}