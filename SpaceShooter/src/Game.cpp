#include "Game.h"
#include "Scripts/Rotator.h"
#include"scripts/CameraRotator.h"

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
    pObject2->Get<Transform>().Position = { 0.0f, 2.0f, 0.0f };
    //pObject->Add<Rotator>(); // Scripting test
    pObject->Add<Mesh>().SetGeoAndMatByName("Cube", "SimpleMaterial");
    pObject2->Add<Mesh>().SetGeoAndMatByName("Sphere", "SimpleMaterial");
    m_GameObjects.emplace_back(pObject);
    m_GameObjects.emplace_back(pObject2);

    // Create Camera
    GameObject* pCamera = new GameObject();
    Transform& camTransform = pCamera->Get<Transform>();
    camTransform.Position = { 0.0f, 0.0f, -5.0f };
    camTransform.Rotation.SetRotationFromAngles(0.0f, 0.0f, 0.0f);
    pCamera->Add<CameraRotator>();
    pCamera->Add<Camera>().IsPrimary = true;
    m_GameObjects.emplace_back(pCamera);
}

void Game::Update(const DXH::Timer& gt)
{
}

void Game::Destroy(const DXH::Timer& gt)
{
    for (auto go : m_GameObjects)
    {
        delete go;
    }
}
