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

	GameObject* pObject = new GameObject();
	GameObject* pObject2 = new GameObject();
	Transform* transform = pObject->Add<Transform>();
	Transform* transform2 = pObject2->Add<Transform>();
	RigidBody* rigidBody = pObject->Add<RigidBody>();
	RigidBody* rigidBody2 = pObject2->Add<RigidBody>();
	auto col1 = pObject->Add<SphereCollider>();
	auto col2 = pObject2->Add<SphereCollider>();
	col1->Radius = 1.0f;
	col2->Radius = 1.0f;
	transform->Position = { 0.f, 1.0f, 0.0f };
	transform2->Position = { -5.0f, 1.0f, 0.0f };
	Mesh* mesh = pObject->Add<Mesh>();
	Mesh* mesh2 = pObject2->Add<Mesh>();
	rigidBody->Velocity = { -500.0f, 0.0f, 0.0f };
	rigidBody2->Velocity = { 500.0f, 0.0f, 0.0f };
	mesh->Geo = RendererResource::GetGeometry("Sphere");
	mesh->Mat = RendererResource::GetMaterial("SimpleMaterial");
	mesh2->Geo = RendererResource::GetGeometry("Sphere");
	mesh2->Mat = RendererResource::GetMaterial("SimpleMaterial");
	m_GameObjects.emplace_back(pObject);
	m_GameObjects.emplace_back(pObject2);

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
