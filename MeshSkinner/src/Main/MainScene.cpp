#include "pch.h"
#include "MainScene.h"

#include "Renderer/Renderer.h"
#include "Renderer/Buffer.h"

// test code taken from https://www.glfw.org/docs/3.3/quick.html

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};

static const char *vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char *fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static GLuint vertex_buffer, vertex_shader, fragment_shader, program;
static GLint mvp_location, vpos_location, vcol_location;

MainScene::MainScene() : Scene()
{

}

MainScene::~MainScene()
{

}

void MainScene::Start()
{
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *)0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *)(sizeof(float) * 2));

    std::vector<StaticVertex> vertices;
    vertices.push_back(StaticVertex(glm::vec3(0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f)));
    vertices.push_back(StaticVertex(glm::vec3(1.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f)));
    vertices.push_back(StaticVertex(glm::vec3(1.f, 0.f, 2.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(1.f)));

    auto vbo = MakeRef<VertexBuffer<StaticVertex>>(StaticVertex::layout);
    vbo->SetData(vertices.data(), vertices.size());
    vbo->SetData(vertices.data(), vertices.size(), 3);

    std::vector<uint32_t> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    auto ibo = MakeRef<IndexBuffer<uint32_t>>();
    ibo->SetData(indices.data(), indices.size());

    auto vao = MakeRef<VertexArray<StaticVertex, uint32_t>>();
    vao->SetVertexBuffer(vbo);
    vao->SetIndexBuffer(ibo);
}

void MainScene::EarlyUpdate()
{

}

void MainScene::Update()
{
    auto bufferSize = Window::GetFramebufferSize();
    auto ratio = bufferSize.x / (float)bufferSize.y;
    glm::mat4 m, p, mvp;
    m = glm::rotate(glm::mat4(1.f), glm::radians((float)glfwGetTime()) * 10.f, glm::vec3(0.f, 0.f, 1.f));
    p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mvp = m * p;

    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)glm::value_ptr(mvp));
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MainScene::UpdateUI()
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
    auto bufferSize = Window::GetFramebufferSize();
    ImGui::Image((void *)(intptr_t)Window::GetFramebufferTexture(), { (float)bufferSize.x, (float)bufferSize.y });
    ImGui::End();
}

void MainScene::LateUpdate()
{

}

void MainScene::End()
{

}
