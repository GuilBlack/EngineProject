#include "Game.h"
#include "scripts/CameraController.h"
#include "scripts/SpaceShip.h"
#include "scripts/Score.h"
#include "scripts/Asteroid.h"
#include "scripts/AsteroidSpawner.h"
#include <DXHCore.h>
#include <DXHRendering.h>
#include "ui/Button.h"
#include "ui/Button.h"
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

    // Create Menu Button
    float height = InputManager::GetInstance().GetNormalizeCoord().x;
    GameObject* pStart = GameObject::Create();
    pStart->Add<Mesh>().SetGeoAndMatByName("Square", "StartButtonMaterial");
    pStart->Add<Button>();
    pStart->SetScale({ 100.f, 100.f, 1.f });
    pStart->SetPosition(0.f, -(height / 3.f), 0.f);
    auto& startButton = pStart->Get<Button>();

    GameObject* pExit = GameObject::Create();
    pExit->Add<Mesh>().SetGeoAndMatByName("Square", "ExitButtonMaterial");
    pExit->Add<Button>();
    pExit->SetScale({ 100.f, 100.f, 1.f });
    pExit->SetPosition(0.f, (height / 3.f), 0.f);
    auto& exitButton = pExit->Get<Button>();

    // set callback start
    startButton.SetStartCallback([]() {
        // Create Skybox
        GameObject* pSkybox = GameObject::Create();
        pSkybox->Add<Mesh>().SetGeoAndMatByName("Sphere", "SkyboxMaterial");
        pSkybox->SetScale({ 5000.f, 5000.f, 5000.f });

        // Create SpaceShip
        GameObject* pSpaceShip = GameObject::Create();
        pSpaceShip->SetPosition(0.f, 0.f, -1.f);
        pSpaceShip->Add<SpaceShip>();
        pSpaceShip->Add<CameraController>();
        pSpaceShip->Add<Camera>().IsPrimary = true;
        pSpaceShip->Add<RigidBody>().Mass = 0.5f;
        auto& c = pSpaceShip->Add<SphereCollider>();
        c.Radius = 1.f;
        //c.CollisionLayer = DXH::CollisionLayer::One;
        //c.CollisionMask = DXH::CollisionLayer::One;

        // Create Score
        GameObject* pScore = GameObject::Create();
        auto& s = pScore->Add<Score>();
        s.SetSpaceShip(pSpaceShip);
        pScore->SetPosition(-.975f, .95f, 0.f);
        NumberUI &num = pScore->Add<NumberUI>();
        num.InitGeometry(5);
        num.Number = "00000";

        // Create asteroid field
        //const size_t asteroidCount = 100;
        GameObject* asteroidSpawnerObj = GameObject::Create();
        AsteroidSpawner&  ap = asteroidSpawnerObj->Add<AsteroidSpawner>();
        ap.SetSpaceShip(pSpaceShip);
        /*for (size_t i = 0; i < asteroidCount; i++)
        {
            Asteroid::CreateAsteroid(pSpaceShip).SetRandomPosition();
        }*/


        //Create crosshair
        GameObject* pCrossHair =  GameObject::Create();
        pCrossHair->Add<Mesh>().SetGeoAndMatByName("Square", "UI_Material");
        pCrossHair->SetScale({ 10.f, 10.f, 1.f });
    });
    //Set exit call back
    exitButton.SetExitCallback([]() {DXHEngine::GetInstance().Shutdown(); });
}

void Game::Destroy(const DXH::Timer& gt)
{
}

void Game::LoadAssets()
{
    using namespace DXH;
    // Create textures
    RendererResource::CreateTexture("AsteroidTexture", L"res/textures/asteroid.dds");
    RendererResource::CreateTexture("CrossHair_Texture", L"res/textures/crosshair.dds");
    RendererResource::CreateTexture("ExitButtonTexture", L"res/textures/exit.dds");
    RendererResource::CreateTexture("StartButtonTexture", L"res/textures/start.dds");
    RendererResource::CreateTextureCube("SkyboxTexture", L"res/textures/space-cubemap.dds");
    
    // Create shaders
    RendererResource::CreateShader("SkyboxShader",
        "res/shaders/compiled/skybox-vs.cso",
        "res/shaders/compiled/skybox-ps.cso",
        ShaderProgramType::SkyboxShader,
        InputLayoutType::PositionNormalTexcoord
    );

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
        "SkyboxMaterial",
        MaterialType::Skybox,
        "SkyboxShader"
    );
    SkyboxMaterial* pSkyboxMaterial =
        dynamic_cast<SkyboxMaterial*>(RendererResource::GetInstance().GetMaterial("SkyboxMaterial"));
    pSkyboxMaterial->SkyboxTexture = RendererResource::GetInstance().GetTextureCube("SkyboxTexture");

    RendererResource::CreateMaterial(
        "RedLightingMaterial",
        MaterialType::Lighting, "BasicLightingShader"
    );
    SimpleLightingMaterial* pRedLightingMaterial =
        dynamic_cast<SimpleLightingMaterial*>(RendererResource::GetInstance().GetMaterial("RedLightingMaterial"));
    pRedLightingMaterial->DiffuseAlbedo = { 1.0f, 0.0f, 0.0f, 1.0f };
    pRedLightingMaterial->FresnelR0 = { 0.01f, 0.01f, 0.01f };
    pRedLightingMaterial->Roughness = 0.5f;

    RendererResource::CreateMaterial(
        "AsteroidMaterial", MaterialType::TextureLighting,
        "TextureLightingShader"
    );

    RendererResource::CreateMaterial(
        "StartButtonMaterial",
        MaterialType::TextureLighting,
        "UI_Shader"
    );

    RendererResource::CreateMaterial(
        "ExitButtonMaterial",
        MaterialType::TextureLighting,
        "UI_Shader"
    );

    RendererResource::CreateMaterial(
        "UI_Material",
        MaterialType::TextureLighting,
        "UI_Shader"
    );

    TextureLightingMaterial* pAsteroidMaterial =
        dynamic_cast<TextureLightingMaterial*>(RendererResource::GetInstance().GetMaterial("AsteroidMaterial"));
    pAsteroidMaterial->DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    pAsteroidMaterial->FresnelR0 = { 0.01f, 0.01f, 0.01f };
    pAsteroidMaterial->Roughness = 0.5f;
    pAsteroidMaterial->DiffuseTexture = RendererResource::GetInstance().GetTexture("AsteroidTexture");

    TextureLightingMaterial* pUIMaterial =
        dynamic_cast<TextureLightingMaterial*>(RendererResource::GetInstance().GetMaterial("UI_Material"));
    pUIMaterial->DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    pUIMaterial->FresnelR0 = { 0.01f, 0.01f, 0.01f };
    pUIMaterial->Roughness = 0.5f;
    pUIMaterial->DiffuseTexture = RendererResource::GetInstance().GetTexture("CrossHair_Texture");

    TextureLightingMaterial* pStartButton =
        dynamic_cast<TextureLightingMaterial*>(RendererResource::GetInstance().GetMaterial("StartButtonMaterial"));
    pStartButton->DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    pStartButton->FresnelR0 = { 0.01f,0.01f,0.01f };
    pStartButton->Roughness = 0.0f;
    pStartButton->DiffuseTexture = RendererResource::GetInstance().GetTexture("StartButtonTexture");

    TextureLightingMaterial* pExitButton =
        dynamic_cast<TextureLightingMaterial*>(RendererResource::GetInstance().GetMaterial("ExitButtonMaterial"));
    pExitButton->DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    pExitButton->FresnelR0 = { 0.01f,0.01f,0.01f };
    pExitButton->Roughness = 0.0f;
    pExitButton->DiffuseTexture = RendererResource::GetInstance().GetTexture("ExitButtonTexture");
}
