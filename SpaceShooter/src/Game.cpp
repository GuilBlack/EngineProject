#include "Game.h"

void Game::StartEngine()
{
	using namespace DXH;
	DXHEngine::GetInstance().Init(AppProperties{
			.WindowTitle = L"Space Shooter",
		},
		[](const Timer& gt) { GetInstance().Init(gt); },
		[](const Timer& gt) { GetInstance().Update(gt); },
		[](const Timer& gt) { GetInstance().Destroy(gt); });
	DXHEngine::GetInstance().Run();
}

void Game::Init(const DXH::Timer& gt)
{
	using namespace DXH;

	for (int i = 0; i < MAX_GO_COUNT; i++)
	{
		GameObject* pObject = new GameObject();
		Transform* transform = pObject->Add<Transform>();
		RigidBody* rigidBody = pObject->Add<RigidBody>();
		auto col1 = pObject->Add<SphereCollider>();
		col1->Radius = 1.f;
		Mesh* mesh = pObject->Add<Mesh>();
		//rigidBody->Velocity = { -100.0f, 0.0f, 0.0f };
		mesh->Geo = RendererResource::GetGeometry("Sphere");
		mesh->Mat = RendererResource::GetMaterial("SimpleMaterial");
		m_GameObjects.emplace_back(pObject);

	}

}

void Game::Update(const DXH::Timer& gt)
{
	gt.DeltaTime();
}

void Game::Destroy(const DXH::Timer& gt)
{
	for (auto go : m_GameObjects)
	{
		delete go;
	}
}
