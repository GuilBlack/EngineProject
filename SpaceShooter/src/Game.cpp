#include <DXHInclude.h>
#include "Game.h"

void Game::StartEngine()
{
	using namespace DXH;
	DXHEngine::GetInstance().Init(AppProperties{
			.WindowTitle = L"Space Shooter",
		},
		[](const Timer& gt) { GetInstance().Update(gt); },
		[](const Timer& gt) { GetInstance().Update(gt); });
	DXHEngine::GetInstance().Run();
}

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(const DXH::Timer& gt)
{
	using namespace DXH;
	GameObject* pObject = new GameObject();
	pObject->Add<Transform>();
	pObject->Remove<Transform>();
	pObject->Add<Transform>();
	delete pObject;
}

void Game::Update(const DXH::Timer& gt)
{
	gt.DeltaTime();
}