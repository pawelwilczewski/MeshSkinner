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
static std::vector<StaticVertex> vertices;
static std::vector<uint32_t> indices;

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
    vertices.push_back(StaticVertex(glm::vec3(-0.6f, -0.4f, 0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f, 0.5f, 1.f)));
    vertices.push_back(StaticVertex(glm::vec3(0.6f, -0.4f, 0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f, 0.3f, 0.f)));
    vertices.push_back(StaticVertex(glm::vec3(0.f, 0.6f, 0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f, 0.f, 0.8f)));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    //indices.push_back(3);
    //indices.push_back(4);
    //indices.push_back(5);

    vbo = MakeRef<VertexBuffer<StaticVertex>>(StaticVertex::layout);
    vbo->SetData(vertices.data(), vertices.size());
    //vbo->SetData(vertices.data(), vertices.size(), 3);

    ibo = MakeRef<IndexBuffer<uint32_t>>();
    ibo->SetData(indices.data(), indices.size());

    vao = MakeRef<VertexArray<StaticVertex, uint32_t>>();
    vao->SetVertexBuffer(vbo);
    vao->SetIndexBuffer(ibo);

    shader = MakeRef<Shader>("UnlitDebug", "assets/shaders/UnlitDebug.vert", "assets/shaders/UnlitDebug.frag");
}

void MainScene::OnEarlyUpdate()
{
    Log::Info("{}", sizeof Vertex);
}

void MainScene::OnUpdate()
{
    vao->Bind();
    shader->Bind();
    shader->UploadUniformMat4("u_ViewProjection", camera->GetViewProjectionMatrix());

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
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

    // setup dockspace window
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    // setup dockspace style
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // setup window flags
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // create the dockspace
    ImGui::Begin("Dockspace", NULL, windowFlags);
    ImGui::PopStyleVar();
    ImGui::DockSpace(ImGui::GetID("Main Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::End();
    ImGui::PopStyleVar(2);

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

    ImGui::Begin("Viewport");
    auto bufferSize = UserInterface::GetViewportSize();
    ImGui::Image((void *)(intptr_t)Window::GetFramebufferTexture(), { (float)bufferSize.x, (float)bufferSize.y });
    ImGui::End();
}

void MainScene::OnLateUpdate()
{

}

void MainScene::OnEnd()
{

}
