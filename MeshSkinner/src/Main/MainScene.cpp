#include "pch.h"
#include "MainScene.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Buffer/Buffer.h"
#include "Core/Camera/Camera.h"
#include "Core/Camera/CameraController.h"

static Ref<Camera> camera;
static Ref<CameraController> cameraController;
static Ref<VertexArray<uint32_t>> vao;
static Ref<VertexBuffer<StaticVertex>> vbo;
static Ref<IndexBuffer<uint32_t>> ibo;
static Ref<Shader> shader;

static Ref<Entity> noneEntity;
static Ref<Entity> staticEntity;
static Ref<Entity> staticEntity2;
static Ref<Entity> staticEntity3;
static Ref<Entity> skeletalEntity;
static Ref<Entity> skeletalEntity2;
static Ref<Entity> staticSkeletalEntity;

MainScene::MainScene() : Scene()
{
    camera = MakeRef<Camera>();
    cameraController = MakeRef<CameraController>(camera);

    Renderer::activeCamera = camera;

    ShaderLibrary::Load("UnlitDebug", "assets/shaders/UnlitDebug.vert", "assets/shaders/UnlitDebug.frag");
}

MainScene::~MainScene()
{
    
}

void MainScene::OnStart()
{
    std::vector<StaticVertex> staticVertices;
    staticVertices.push_back(StaticVertex(glm::vec3(-0.6f, -0.4f, -0.1f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f, 0.5f, 1.f)));
    staticVertices.push_back(StaticVertex(glm::vec3(0.6f, -0.4f, 0.3f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f, 0.3f, 0.f)));
    staticVertices.push_back(StaticVertex(glm::vec3(0.f, 0.6f, 0.5f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f, 0.f, 0.8f)));
    staticVertices.push_back(StaticVertex(glm::vec3(0.4f, -0.6f, -0.4f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f, 0.3f, 1.f)));

    std::vector<SkeletalVertex> skeletalVertices;
    skeletalVertices.push_back(SkeletalVertex(glm::vec3(-2.8f, -0.5f, -1.1f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.0f, 1.0f, 0.f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));
    skeletalVertices.push_back(SkeletalVertex(glm::vec3(-1.6f, -0.7f, -2.1f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));
    skeletalVertices.push_back(SkeletalVertex(glm::vec3(-0.6f, 3.4f, -1.1f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));
    skeletalVertices.push_back(SkeletalVertex(glm::vec3(-1.2f, 0.4f, -4.1f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.0f, 0.0f, 1.f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));

    std::vector<uint32_t> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    auto staticMaterial = MakeRef<Material>();
    staticMaterial->shader = ShaderLibrary::Get("UnlitDebug");
    auto skeletalMaterial = MakeRef<Material>();
    skeletalMaterial->shader = ShaderLibrary::Get("UnlitDebug");
    auto staticMesh = MakeRef<StaticMesh>(staticVertices, indices, staticMaterial, true);
    auto skeletalMesh = MakeRef<SkeletalMesh>(skeletalVertices, indices, skeletalMaterial, true);

    noneEntity = MakeRef<Entity>();

    staticEntity = MakeRef<Entity>(Transform(glm::vec3(0.f, 0.f, 2.f)));
    staticEntity->AddComponent(staticMesh);

    staticEntity2 = MakeRef<Entity>(Transform(glm::vec3(0.f, 0.f, -2.f)));
    staticEntity2->AddComponent(staticMesh);

    staticEntity3 = MakeRef<Entity>(Transform(glm::vec3(0.f, 1.f, 2.f)));
    staticEntity3->AddComponent(staticMesh);
    //staticEntity3->AddComponent(skeletalMesh);

    skeletalEntity = MakeRef<Entity>(Transform(glm::vec3(2.f, 0.f, 0.f)));
    skeletalEntity->AddComponent(skeletalMesh);

    skeletalEntity2 = MakeRef<Entity>(Transform(glm::vec3(-2.f, 0.f, 0.f)));
    skeletalEntity2->AddComponent(skeletalMesh);

    staticSkeletalEntity = MakeRef<Entity>();
    staticSkeletalEntity->transform.SetPosition({ -2.f, 2.f, 0.f });
    staticSkeletalEntity->AddComponent(staticMesh);
    staticSkeletalEntity->AddComponent(skeletalMesh);

    Renderer::Submit(noneEntity);
    Renderer::Submit(staticEntity);
    Renderer::Submit(staticEntity2);
    Renderer::Submit(staticEntity3);
    Renderer::Submit(skeletalEntity);
    Renderer::Submit(skeletalEntity2);
    Renderer::Submit(staticSkeletalEntity);
}

void MainScene::OnEarlyUpdate()
{

}

void MainScene::OnUpdate()
{
    staticEntity->transform.SetPosition(staticEntity->transform.GetPosition() + glm::vec3(0.1f) * Time::GetDeltaSeconds());
    staticEntity2->transform.SetPosition(staticEntity2->transform.GetPosition() + glm::vec3(0.1f) * Time::GetDeltaSeconds());
    staticEntity3->transform.SetPosition(staticEntity3->transform.GetPosition() + glm::vec3(-0.1f) * Time::GetDeltaSeconds());
    staticSkeletalEntity->transform.SetPosition(staticSkeletalEntity->transform.GetPosition() + glm::vec3(-0.1f) * Time::GetDeltaSeconds());

    staticEntity->transform.SetScale(glm::vec3(glm::sin(Time::GetTimeSeconds())));
}

void MainScene::OnUpdateUI()
{
    // debug fps info
    static int updates = 0;
    static float frameTimes = 0.f;
    static float fps = 0.f;
    updates++;
    frameTimes += Time::GetDeltaSeconds();
    fps += Time::GetFPS();

    // debug panel
    ImGui::Begin("Debug info");
    ImGui::Text("Scene info:");
    ImGui::Text("FPS:            %f", Time::GetFPS());
    ImGui::Text("Frame time:     %f ms", Time::GetDeltaSeconds() * 1000.f);
    ImGui::Text("Avg FPS:        %f", fps / updates);
    ImGui::Text("Avg frame time: %f ms", frameTimes / updates * 1000.f);
    ImGui::End();

    // debug panel 2
    ImGui::Begin("Hello info");
    ImGui::Text("Scene info 2:");
    ImGui::Text("FPS:            %f", Time::GetFPS());
    ImGui::End();
}

void MainScene::OnLateUpdate()
{

}

void MainScene::OnEnd()
{

}
