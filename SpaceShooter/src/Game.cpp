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

    LoadAssets();

    // Create Camera
    GameObject* pCamera = new GameObject();
    Transform& camTransform = pCamera->Get<Transform>();
    camTransform.Position = { 0.0f, 0.0f, -5.0f };
    camTransform.Rotation.SetEulerAngles(0.0f, 0.0f, 0.0f);
    pCamera->Add<Controller>();
    pCamera->Add<Camera>().IsPrimary = true;
    m_GameObjects.emplace_back(pCamera);

    // Create objects
    for (int i = 0; i < 500; ++i)
    {
        GameObject* pObject = new GameObject();
        float randX = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
        float randY = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
        float randZ = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
        pObject->Get<Transform>().Position = { randX, randY, randZ };
        pObject->Add<Mesh>().SetGeoAndMatByName("Sphere", "RedLightingMaterial");
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

void Game::LoadAssets()
{
    using namespace DXH;
    // Create shaders
    RendererResource::GetInstance().CreateShader(
        "BasicPhongShader", 
        "res/shaders/compiled/simple-lighting-vs.cso", 
        "res/shaders/compiled/simple-lighting-ps.cso", 
        ShaderProgramType::BasicPhongShader, 
        InputLayoutType::PositionNormal
    );

    // Create materials
    RendererResource::GetInstance().CreateMaterial(
        "RedLightingMaterial", 
        MaterialType::Lighting, "BasicPhongShader"
    );
    SimplePhongMaterial* pRedLightingMaterial = 
        dynamic_cast<SimplePhongMaterial*>(RendererResource::GetInstance().GetMaterial("RedLightingMaterial"));

    pRedLightingMaterial->DiffuseAlbedo = { 1.0f, 0.0f, 0.0f, 1.0f };
    pRedLightingMaterial->FresnelR0 = { 0.01f, 0.01f, 0.01f };
    pRedLightingMaterial->Roughness = 0.25f;
}
