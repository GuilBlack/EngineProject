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

    for (int i = 0; i < 500; ++i)
    {
        GameObject* pObject = new GameObject();
        float randX = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
        float randY = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
        float randZ = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
        pObject->Get<Transform>().Position = { randX, randY, randZ };
        pObject->Add<Mesh>().SetGeoAndMatByName("Cube", "SimpleMaterial");
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
