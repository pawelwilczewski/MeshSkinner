#include "pch.h"
#include "MainScene.h"

#include "Renderer/Renderer.h"
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Renderer/CameraController.h"

static Ref<Camera> camera;
static Ref<CameraController> cameraController;
static Ref<VertexArray<StaticVertex, uint32_t>> vao;
static Ref<VertexBuffer<StaticVertex>> vbo;
static Ref<IndexBuffer<uint32_t>> ibo;
static Ref<Shader> shader;

static Ref<Entity> noneEntity;
static Ref<Entity> staticEntity;
static Ref<Entity> skeletalEntity;
static Ref<Entity> staticSkeletalEntity;

MainScene::MainScene() : Scene()
{
    camera = MakeRef<Camera>();
    cameraController = MakeRef<CameraController>(camera);
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
    staticVertices.push_back(SkeletalVertex(glm::vec3(-2.8f, -0.5f, -1.1f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.6f, 0.5f, 0.f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));
    staticVertices.push_back(SkeletalVertex(glm::vec3(-1.6f, -0.7f, -2.1f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f, 0.5f, 0.2f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));
    staticVertices.push_back(SkeletalVertex(glm::vec3(-0.6f, 3.4f, -1.1f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.3f, 0.5f, 1.f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));
    staticVertices.push_back(SkeletalVertex(glm::vec3(-1.2f, 0.4f, -4.1f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.5f, 0.5f, 0.f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));

    std::vector<uint32_t> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    //vbo = MakeRef<VertexBuffer<StaticVertex>>(StaticVertex::layout);
    //vbo->SetData(staticVertices.data(), staticVertices.size());

    //ibo = MakeRef<IndexBuffer<uint32_t>>();
    //ibo->SetData(indices.data(), indices.size());

    //vao = MakeRef<VertexArray<StaticVertex, uint32_t>>();
    //vao->SetVertexBuffer(vbo);
    //vao->SetIndexBuffer(ibo);

    //shader = MakeRef<Shader>("UnlitDebug", "assets/shaders/UnlitDebug.vert", "assets/shaders/UnlitDebug.frag");
    auto staticMaterial = MakeRef<Material>();
    staticMaterial->shader = MakeRef<Shader>("UnlitDebug", "assets/shaders/UnlitDebug.vert", "assets/shaders/UnlitDebug.frag");
    auto skeletalMaterial = MakeRef<Material>();
    skeletalMaterial->shader = MakeRef<Shader>("UnlitDebug", "assets/shaders/UnlitDebug.vert", "assets/shaders/UnlitDebug.frag");
    auto staticMesh = MakeRef<StaticMesh>(staticVertices, indices, staticMaterial, true);
    auto skeletalMesh = MakeRef<SkeletalMesh>(skeletalVertices, indices, skeletalMaterial, true);

    noneEntity = MakeRef<Entity>();

    staticEntity = MakeRef<Entity>();
    staticEntity->AddComponent(staticMesh);

    skeletalEntity = MakeRef<Entity>();
    skeletalEntity->AddComponent(skeletalMesh);

    staticSkeletalEntity = MakeRef<Entity>();
    staticSkeletalEntity->AddComponent(staticMesh);
    staticSkeletalEntity->AddComponent(skeletalMesh);

    Renderer::Submit(noneEntity);
    Renderer::Submit(staticEntity);
    Renderer::Submit(skeletalEntity);
    Renderer::Submit(staticSkeletalEntity);
}

void MainScene::OnEarlyUpdate()
{

}

void MainScene::OnUpdate()
{
    //vao->Bind();
    //shader->Bind();
    //shader->UploadUniformMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
    
    //glDrawElements(GL_TRIANGLES, vao->GetElementCount(), GL_UNSIGNED_INT, nullptr);
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
