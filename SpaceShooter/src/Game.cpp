#include "Game.h"
#include "scripts/Rotator.h"
#include "scripts/Controller.h"

void Game::StartEngine()
{
    using namespace DXH;
    DXHEngine::GetInstance().Init(AppProperties{
            .WindowTitle = L"Space Shooter",
        },
        [](const Timer& gt) { GetInstance().Init(gt); },
        [](const Timer& gt) { GetInstance().Destroy(gt); });
    DXHEngine::GetInstance().Run();
}

void Game::Init(const DXH::Timer& gt)
{
    using namespace DXH;
    // Create Camera
    GameObject* pCamera = new GameObject();
    Transform& camTransform = pCamera->Get<Transform>();
    camTransform.Position = { 0.0f, 0.0f, -5.0f };
    camTransform.Rotation.SetEulerAngles(0.0f, 0.0f, 0.0f);
    pCamera->Add<Controller>();
    pCamera->Add<Camera>().IsPrimary = true;
    m_GameObjects.emplace_back(pCamera);


    for (int i = 0; i < 100; ++i)
    {
        GameObject* pObject = new GameObject();
        pObject->Get<Transform>().Position = { 0.0f, 0.0f, 0.0f };
        pObject->Add<SphereCollider>().Radius = 1.f;
        pObject->Add<RigidBody>(); 
        pObject->Add<Mesh>().SetGeoAndMatByName("Sphere", "SimpleMaterial");
        m_GameObjects.emplace_back(pObject);
    }

}

void Game::Destroy(const DXH::Timer& gt)
{
    for (auto go : m_GameObjects)
    {
        delete go;
    }
}
