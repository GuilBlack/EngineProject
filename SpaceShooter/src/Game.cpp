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
        pObject->Add<Mesh>().SetGeoAndMatByName("Sphere", "AsteroidMaterial");
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
    // Create textures
    RendererResource::GetInstance().CreateTexture("AsteroidTexture", L"res/textures/asteroid.dds");

    // Create shaders
    RendererResource::GetInstance().CreateShader(
        "BasicLightingShader", 
        "res/shaders/compiled/simple-lighting-vs.cso", 
        "res/shaders/compiled/simple-lighting-ps.cso", 
        ShaderProgramType::BasicLightingShader, 
        InputLayoutType::PositionNormalTexcoord
    );
    RendererResource::GetInstance().CreateShader(
        "TextureLightingShader",
        "res/shaders/compiled/texture-lighting-vs.cso",
        "res/shaders/compiled/texture-lighting-ps.cso",
        ShaderProgramType::TextureLightingShader,
        InputLayoutType::PositionNormalTexcoord
    );

    // Create materials
    RendererResource::GetInstance().CreateMaterial(
        "RedLightingMaterial", 
        MaterialType::Lighting, "BasicLightingShader"
    );
    //SimpleLightingMaterial* pRedLightingMaterial = 
    //    dynamic_cast<SimpleLightingMaterial*>(RendererResource::GetInstance().GetMaterial("RedLightingMaterial"));
    //pRedLightingMaterial->DiffuseAlbedo = { 1.0f, 0.0f, 0.0f, 1.0f };
    //pRedLightingMaterial->FresnelR0 = { 1.01f, 1.01f, 0.01f };
    //pRedLightingMaterial->Roughness = 0.5f;

    RendererResource::GetInstance().CreateMaterial(
        "AsteroidMaterial", MaterialType::TextureLighting,
        "TextureLightingShader"
    );

    TextureLightingMaterial* pAsteroidMaterial =
        dynamic_cast<TextureLightingMaterial*>(RendererResource::GetInstance().GetMaterial("AsteroidMaterial"));
    pAsteroidMaterial->DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    pAsteroidMaterial->FresnelR0 = { 0.01f, 0.01f, 0.01f };
    pAsteroidMaterial->Roughness = 0.5f;
    pAsteroidMaterial->DiffuseTexture = RendererResource::GetInstance().GetTexture("AsteroidTexture");
}
