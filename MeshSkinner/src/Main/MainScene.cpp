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

static Ref<SkeletalMesh> editedMesh;

static float brushRadius = 10.f;

static bool isInteractingWithImGui = false;
static bool clickedInViewport = false;

MainScene::MainScene() : Scene()
{
    camera = MakeRef<Camera>("MainCamera");
    cameraController = MakeRef<CameraController>(camera, 10.f);

    Renderer::activeCamera = camera;

    ShaderLibrary::Load("Bone", "assets/shaders/Bone.vert", "assets/shaders/Bone.frag", 1);
    ShaderLibrary::Load("WeightPaint", "assets/shaders/WeightPaint.vert", "assets/shaders/WeightPaint.frag", 0);

    onMouseMovedCallback = MakeCallbackRef<glm::vec2>([&](const glm::vec2 &position) { OnMouseMoved(position); });
    onMouseButtonPressedCallback = MakeCallbackRef<int>([&](int button) { OnMouseButtonPressed(button); });

    Input::OnMouseMovedSubscribe(onMouseMovedCallback);
    Input::OnMouseButtonPressedSubscribe(onMouseButtonPressedCallback);
}

MainScene::~MainScene()
{
    Input::OnMouseMovedUnsubscribe(onMouseMovedCallback);
    Input::OnMouseButtonPressedUnsubscribe(onMouseButtonPressedCallback);
}

void MainScene::OnStart()
{
    std::vector<StaticVertex> staticVertices;
    staticVertices.push_back(StaticVertex(glm::vec3(-0.6f, -0.4f, -0.1f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(0.f, 0.5f, 1.f)));
    staticVertices.push_back(StaticVertex(glm::vec3(0.6f, -0.4f, 0.3f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(1.f, 0.3f, 0.f)));
    staticVertices.push_back(StaticVertex(glm::vec3(0.f, 0.6f, 0.5f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(1.f, 0.f, 0.8f)));
    staticVertices.push_back(StaticVertex(glm::vec3(0.4f, -0.6f, -0.4f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(0.f, 0.3f, 1.f)));

    std::vector<SkeletalVertex> skeletalVertices;
    skeletalVertices.push_back(SkeletalVertex(glm::vec3(-2.8f, -0.5f, -1.1f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(0.0f, 1.0f, 0.f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));
    skeletalVertices.push_back(SkeletalVertex(glm::vec3(-1.6f, -0.7f, -2.1f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));
    skeletalVertices.push_back(SkeletalVertex(glm::vec3(-0.6f, 3.4f, -1.1f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));
    skeletalVertices.push_back(SkeletalVertex(glm::vec3(-1.2f, 0.4f, -4.1f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(0.0f, 0.0f, 1.f), glm::vec<4, uint16_t>(0, 1, 2, 3), glm::vec4(0.25f)));

    std::vector<uint32_t> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    auto staticMesh = MakeRef<StaticMesh>(staticVertices, indices, MaterialLibrary::GetDefault(), true);
    //auto skeletalMesh = MakeRef<SkeletalMesh>(skeletalVertices, indices, MaterialLibrary::GetDefault(), true);
    auto skeletalMesh = MakeRef<SkeletalMesh>();
    auto rootBone = Ref<Bone>();
    MeshLibrary::Get("assets/models/shark.gltf", skeletalMesh, rootBone);
    skeletalMesh->material = MakeRef<Material>(ShaderLibrary::Get("WeightPaint"));

    noneEntity = MakeRef<Entity>("none");

    staticEntity = MakeRef<Entity>("static", Transform(glm::vec3(0.f, 0.f, 2.f)));
    staticEntity->AddComponent(MeshLibrary::GetCube());

    staticEntity2 = MakeRef<Entity>("static2", Transform(glm::vec3(0.f, 0.f, -2.f)));
    staticEntity2->AddComponent(staticMesh);

    staticEntity3 = MakeRef<Entity>("static 3", Transform(glm::vec3(0.f, 1.f, 2.f)));
    staticEntity3->AddComponent(staticMesh);

    skeletalEntity = MakeRef<Entity>("skeletal", Transform(glm::vec3(15.f, 0.f, 2.f)));
    skeletalEntity->AddComponent(skeletalMesh);
    //skeletalEntity->transform.SetScale(glm::vec3(0.01f));
    skeletalEntity->SetParent(rootBone);
    //rootBone->transform.SetScale(glm::vec3(0.01f));
    rootBone->transform.Translate(glm::vec3(-200.f, 0.f, 0.f));
    //rootBone->transform.Translate(glm::vec3(-500.f, 0.f, 0.f));
    //rootBone->transform.SetScale(glm::vec3(10.f, 10.f, 10.f));

    // add bone meshes
    auto boneMat = MakeRef<Material>(ShaderLibrary::Get("Bone"));
    for (auto &bone : skeletalMesh->skeleton->GetBones())
    {
        // calculate the bone length with some default for tip bones
        auto boneLength = 50.f;
        auto &children = bone->GetChildren();
        if (children.size() == 1)
            boneLength = glm::length(bone->GetChildren().begin()->get()->transform.GetPosition());

        auto boneMesh = MeshLibrary::GetBone(boneLength);
        boneMesh->material = boneMat;
        bone->AddComponent(boneMesh);
    }

    skeletalEntity2 = MakeRef<Entity>("skeletal 2", Transform(glm::vec3(-2.f, 0.f, 0.f)));
    skeletalEntity2->AddComponent(skeletalMesh);

    staticSkeletalEntity = MakeRef<Entity>("static skeletal");
    staticSkeletalEntity->transform.SetPosition({ -2.f, 2.f, 0.f });
    staticSkeletalEntity->AddComponent(staticMesh);
    staticSkeletalEntity->AddComponent(skeletalMesh);

    Renderer::Submit(noneEntity);
    Renderer::Submit(staticEntity);
    Renderer::Submit(staticEntity2);
    Renderer::Submit(staticEntity3);
    Renderer::Submit(skeletalEntity);
    //Renderer::Submit(skeletalEntity2);
    //Renderer::Submit(staticSkeletalEntity);

    editedMesh = skeletalMesh;
}

void MainScene::OnEarlyUpdate()
{

}

void MainScene::OnUpdate()
{
    staticEntity->transform.Translate(glm::vec3(0.1f) * Time::GetDeltaSeconds());
    staticEntity2->transform.Translate(glm::vec3(0.1f) * Time::GetDeltaSeconds());
    staticEntity3->transform.Translate(glm::vec3(-0.1f) * Time::GetDeltaSeconds());
    //staticSkeletalEntity->transform.Translate(glm::vec3(-0.1f) * Time::GetDeltaSeconds());
    //skeletalEntity->transform.Translate(glm::vec3(0.f, -1.f, 0.f) * Time::GetDeltaSeconds());
    //skeletalEntity->transform.Rotate(glm::vec3(30.f, 0.f, 0.f) * Time::GetDeltaSeconds());
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

    isInteractingWithImGui = false;

    // scene stats
    ImGui::Begin("Scene Stats");
    ImGui::Text("FPS:            %f", Time::GetFPS());
    ImGui::Text("Frame time:     %f ms", Time::GetDeltaSeconds() * 1000.f);
    ImGui::Text("Avg FPS:        %f", fps / updates);
    ImGui::Text("Avg frame time: %f ms", frameTimes / updates * 1000.f);
    ImGui::End();

    // edited mesh
    ImGui::Begin("Edited Mesh");
    isInteractingWithImGui |= ImGui::SliderInt("ActiveBone", &Renderer::activeBone, 0, editedMesh->skeleton->GetBones().size() - 1);
    ImGui::End();

    // settings
    ImGui::Begin("Settings");
    isInteractingWithImGui |= ImGui::DragFloat("Mouse sensitivity", &cameraController->mouseSensitivity, 0.0001f, 0.0f, 10.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
    ImGui::End();

    // tool
    ImGui::Begin("Tool");
    isInteractingWithImGui |= ImGui::DragFloat("Brush radius", &brushRadius, 0.1f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic);
    ImGui::End();

    // viewport
    ImGui::Begin("Viewport Settings");
    if (ImGui::Button("Reset camera"))
    {
        isInteractingWithImGui = true;
        camera->transform.SetPosition(glm::vec3(0.f));
        camera->transform.SetRotation(glm::vec3(0.f));
    }
    ImGui::End();
}

void MainScene::OnLateUpdate()
{

}

void MainScene::OnEnd()
{

}

void MainScene::OnMouseMoved(const glm::vec2 &)
{
    if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && clickedInViewport)
    {
        glm::vec3 intersection;
        if (MathUtils::RayMeshIntersection(camera->ProjectViewportToWorld(Input::GetMouseViewportPosition()), skeletalEntity->GetWorldMatrix(), editedMesh, intersection))
        {
            auto ent = MakeRef<Entity>("CubeIntersection", Transform(intersection));
            auto mesh = MeshLibrary::GetCube();
            mesh->material->shader = ShaderLibrary::GetDefaultOverlay();
            ent->AddComponent(mesh);
            Renderer::Submit(ent);
        }
    }
}

void MainScene::OnMouseButtonPressed(int button)
{
    if (button == MOUSE_BUTTON_LEFT)
    {
        clickedInViewport = Input::IsMouseInViewport() && !isInteractingWithImGui;
    }
}
