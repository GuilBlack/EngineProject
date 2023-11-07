#include "Game.h"
#include "scripts/CameraController.h"
#include "scripts/SpaceShip.h"
#include <DXHCore.h>
#include <DXHRendering.h>
using namespace DXH;

void Game::StartEngine()
{
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

    // Create SpaceShip
    GameObject* pSpaceShip = new GameObject();
    pSpaceShip->Add<SpaceShip>();
    pSpaceShip->Add<CameraController>();
    pSpaceShip->Add<Camera>().IsPrimary = true;
    pSpaceShip->Add<RigidBody>().Mass = 0.5f;
    pSpaceShip->Add<SphereCollider>().Radius = 1.f;
    m_GameObjects.push_back(pSpaceShip);
    GameObject* pScore = new GameObject();
    pScore->SetPosition(-1.f, 1.f, 0.f);
    pScore->Add<Mesh>();
    NumberUI& num = pScore->Add<NumberUI>();
    num.InitGeometry(5);
    num.Number = "12345";
    m_GameObjects.push_back(pScore);


    // Create asteroid field
    const size_t asteroidCount = 100;
    for (size_t i = 0; i < asteroidCount; i++)
    {
        GameObject* pAsteroid = new GameObject();
        float randX = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
        float randY = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
        float randZ = ((float)rand() / (float)RAND_MAX - 0.5f) * 100.f;
        pAsteroid->SetPosition(randX, randY, randZ);
        randX = ((float)rand() / (float)RAND_MAX - 0.5f);
        randY = ((float)rand() / (float)RAND_MAX - 0.5f);
        randZ = ((float)rand() / (float)RAND_MAX - 0.5f);
        pAsteroid->Add<RigidBody>().Velocity = {randX, randY, randZ};
        pAsteroid->Add<Mesh>().SetGeoAndMatByName("Sphere", "AsteroidMaterial");
        pAsteroid->Add<SphereCollider>().Radius = 1.f;
        m_GameObjects.push_back(pAsteroid);
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
    RendererResource::CreateTexture("AsteroidTexture", L"res/textures/asteroid.dds");

    // Create shaders
    RendererResource::CreateShader(
        "BasicLightingShader",
        "res/shaders/compiled/simple-lighting-vs.cso",
        "res/shaders/compiled/simple-lighting-ps.cso",
        ShaderProgramType::BasicLightingShader,
        InputLayoutType::PositionNormalTexcoord
    );

    RendererResource::CreateShader(
        "TextureLightingShader",
        "res/shaders/compiled/texture-lighting-vs.cso",
        "res/shaders/compiled/texture-lighting-ps.cso",
        ShaderProgramType::TextureLightingShader,
        InputLayoutType::PositionNormalTexcoord
    );

    RendererResource::CreateShader(
        "UI_Shader",
        "res/shaders/compiled/ui-shader-vs.cso",
        "res/shaders/compiled/ui-shader-ps.cso",
        ShaderProgramType::TextureLightingShader,
        InputLayoutType::PositionNormalTexcoord
    );

    // Create materials
    RendererResource::CreateMaterial(
        "RedLightingMaterial",
        MaterialType::Lighting, "BasicLightingShader"
    );
    SimpleLightingMaterial* pRedLightingMaterial =
        dynamic_cast<SimpleLightingMaterial*>(RendererResource::GetInstance().GetMaterial("RedLightingMaterial"));
    pRedLightingMaterial->DiffuseAlbedo = {1.0f, 0.0f, 0.0f, 1.0f};
    pRedLightingMaterial->FresnelR0 = {0.01f, 0.01f, 0.01f};
    pRedLightingMaterial->Roughness = 0.5f;

    RendererResource::CreateMaterial(
        "AsteroidMaterial", MaterialType::TextureLighting,
        "TextureLightingShader"
    );

    RendererResource::CreateMaterial(
        "UI_Material", MaterialType::TextureLighting,
        "UI_Shader"
    );

    TextureLightingMaterial* pAsteroidMaterial =
        dynamic_cast<TextureLightingMaterial*>(RendererResource::GetInstance().GetMaterial("AsteroidMaterial"));
    pAsteroidMaterial->DiffuseAlbedo = {1.0f, 1.0f, 1.0f, 1.0f};
    pAsteroidMaterial->FresnelR0 = {0.01f, 0.01f, 0.01f};
    pAsteroidMaterial->Roughness = 0.5f;
    pAsteroidMaterial->DiffuseTexture = RendererResource::GetInstance().GetTexture("AsteroidTexture");
}
