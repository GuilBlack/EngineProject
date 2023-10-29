#include <DXHInclude.h>
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
	using namespace DXH;
	GameObject* pObject = new GameObject();
	pObject->Add<Transform>();
	pObject->Remove<Transform>();
	pObject->Add<Transform>();
	delete pObject;
}

Game::~Game()
{
}

void Game::Update(const DXH::Timer& gt)
{
	gt.DeltaTime();
}