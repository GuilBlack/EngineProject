#include "Game.h"
#include "src/ecs/components/Physics.h"

// The entry point of the game
DXH::DXHEngine* DXH::CreateDXHEngine()
{
	DXH::AppProperties props
	{
		.WindowTitle = L"Space Shooter"
	};
	return new Game(props);
}

Game::Game(DXH::AppProperties props) : DXH::DXHEngine(props), testEntity()
{
	// TODO For testing purposes, remove later
	testEntity.AddComponent<DXH::RigidBody>();
	assert(testEntity.HasComponent<DXH::RigidBody>());
}

Game::~Game()
{
}

void Game::Update(const DXH::Timer& gt)
{
}