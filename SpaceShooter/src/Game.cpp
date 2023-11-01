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
	Transform& t1 = *(pObject->Add<Transform>());
	t1.Position = { 0.0f, -4.0f, 0.0f };

	Transform* transform = pObject2->Add<Transform>();
	Mesh* mesh = pObject->Add<Mesh>();
	Mesh* mesh2 = pObject2->Add<Mesh>();
	mesh->Geo = RendererResource::GetGeometry("Cube");
	mesh->Mat = RendererResource::GetMaterial("SimpleMaterial");
	mesh2->Geo = RendererResource::GetGeometry("Sphere");
	mesh2->Mat = RendererResource::GetMaterial("SimpleMaterial");
	m_GameObjects.emplace_back(pObject);
	m_GameObjects.emplace_back(pObject2);

	// Create Camera
	GameObject* pCamera = new GameObject();
	Transform& camTransform = *(pCamera->Add<Transform>());
	camTransform.Position = { 0.0f, 0.0f, -5.0f };
	camTransform.Rotation.SetRotationFromAngles(0.0f, 0.0f, 0.0f);
	Camera& cam = *(pCamera->Add<Camera>());
	cam.IsPrimary = true;
	m_GameObjects.emplace_back(pCamera);
}

void Game::Update(const DXH::Timer& gt)
{
	DXH::Transform* transform = m_GameObjects[0]->Get<DXH::Transform>();
	transform->Position.y = sinf(gt.TotalTime()) * 5.0f;
	DXH::Transform* camTransform = m_GameObjects[2]->Get<DXH::Transform>();
	camTransform->Rotation.SetRotationFromAngles(gt.TotalTime() * 60.f, 0.f, 0.f);

	gt.DeltaTime();
}

void Game::Destroy(const DXH::Timer& gt)
{
	for (auto go : m_GameObjects)
	{
		delete go;
	}
}
