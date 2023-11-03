#include "Game.h"
#include "Scripts/Rotator.h"

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

	/*for (int i = 0; i < 2; i++)
	{

	}*/

		GameObject* pObject = new GameObject();
		auto& transform = pObject->Get<Transform>();
		pObject->Get<Transform>().Position = { -3.f, 0.0f, 0.0f};
		auto& rigidBody = pObject->Add<RigidBody>();
		auto& col1 = pObject->Add<SphereCollider>();
		col1.Radius = 1.f;
		auto& mesh = pObject->Add<Mesh>();
		rigidBody.Velocity = { 500.0f, 0.0f, 0.0f };
		mesh.Geo = RendererResource::GetGeometry("Sphere");
		mesh.Mat = RendererResource::GetMaterial("SimpleMaterial");
		m_GameObjects.emplace_back(pObject);


		pObject = new GameObject();
		auto& transform2 = pObject->Get<Transform>();
		pObject->Get<Transform>().Position = { 3.f, 0.0f, 0.0f };
		auto & rigidbody2 = pObject->Add<RigidBody>();
		auto& col2 = pObject->Add<SphereCollider>();
		col2.Radius = 1.f;
		auto& mesh2 = pObject->Add<Mesh>();
		rigidbody2.Velocity = { -500.0f, 0.0f, 0.0f };
		mesh2.Geo = RendererResource::GetGeometry("Sphere");
		mesh2.Mat = RendererResource::GetMaterial("SimpleMaterial");
		m_GameObjects.emplace_back(pObject);
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
