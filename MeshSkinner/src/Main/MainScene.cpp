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
    std::vector<StaticVertex> vertices;

    vertices.push_back(StaticVertex(glm::vec3(-0.6f, -0.4f, -0.1f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f, 0.5f, 1.f)));
    vertices.push_back(StaticVertex(glm::vec3(0.6f, -0.4f, 0.3f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f, 0.3f, 0.f)));
    vertices.push_back(StaticVertex(glm::vec3(0.f, 0.6f, 0.5f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f, 0.f, 0.8f)));
    vertices.push_back(StaticVertex(glm::vec3(0.4f, -0.6f, -0.4f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f, 0.3f, 1.f)));

    vbo = MakeRef<VertexBuffer<StaticVertex>>(StaticVertex::layout);
    vbo->SetData(vertices.data(), vertices.size());
    //vbo->SetData(vertices.data(), vertices.size(), 4);

    std::vector<uint32_t> indices;

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(3);

    ibo = MakeRef<IndexBuffer<uint32_t>>();
    ibo->SetData(indices.data(), indices.size());

    vao = MakeRef<VertexArray<StaticVertex, uint32_t>>();
    vao->SetVertexBuffer(vbo);
    vao->SetIndexBuffer(ibo);

    shader = MakeRef<Shader>("UnlitDebug", "assets/shaders/UnlitDebug.vert", "assets/shaders/UnlitDebug.frag");
}

void MainScene::OnEarlyUpdate()
{

}

void MainScene::OnUpdate()
{
    vao->Bind();
    shader->Bind();
    shader->UploadUniformMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
    
    glDrawElements(GL_TRIANGLES, vao->GetElementCount(), GL_UNSIGNED_INT, nullptr);
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
