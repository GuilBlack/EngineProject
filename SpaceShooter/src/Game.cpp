#include "Game.h"

void Game::StartEngine()
{
	using namespace DXH;
	DXHEngine::GetInstance().Init(AppProperties{
			.WindowTitle = L"Space Shooter",
		}, [](const Timer& gt) { GetInstance().Update(gt); });
	DXHEngine::GetInstance().Run();
}

Game::Game()
{
}

Game::~Game()
{
}

void Game::Update(const DXH::Timer& gt)
{
	gt.DeltaTime();
}