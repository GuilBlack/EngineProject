#include "Game.h"

// The entry point of the game
DXH::DXHEngine* DXH::CreateDXHEngine()
{
	DXH::AppProperties props
	{
		.WindowTitle = L"Space Shooter"
	};
	return new Game(props);
}

Game::Game(DXH::AppProperties props) : DXH::DXHEngine(props)
{
}

Game::~Game()
{
}
