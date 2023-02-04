#include "pch.h"
#include "Application.h"
#include "Error.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

GLFWwindow *Application::s_Window;

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

void Application::Init(uint32_t width, uint32_t height, const char *title)
{
    Log::Init();

    glfwSetErrorCallback(Error::CallbackGLFW);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    s_Window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!s_Window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(s_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Log::Critical("Glad loader failure!");
        exit(EXIT_FAILURE);
    }

    Log::Info("OpenGL Renderer:");
    Log::Info("    Vendor: {0}", (const char *)glGetString(GL_VENDOR));
    Log::Info("    Renderer: {0}", (const char *)glGetString(GL_RENDERER));
    Log::Info("    Version: {0}", (const char *)glGetString(GL_VERSION));

    // enable gl debug messages
#if defined DEBUG || defined RELEASE
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(Error::CallbackGL, nullptr);
#endif

    glfwSwapInterval(1);

    Log::Info("Application init successful...");

    Input::Init();
    auto func = MakeCallbackRef<int>([&](int key) { if (key == KEY_F) Log::Info("{0}", key); });
    auto funcWindow = MakeCallbackRef<glm::ivec2>([&](glm::ivec2 windowSize) { Log::Info("{0}", windowSize); });
    auto funcClose = MakeCallbackRef<int>([&](int key) { if (key == KEY_ESCAPE) glfwSetWindowShouldClose(s_Window, GLFW_TRUE); });
    Input::OnKeyPressedSubscribe(func);
    Input::OnKeyPressedSubscribe(funcClose);
    Input::OnKeyReleasedSubscribe(func);
    Input::OnWindowResizedSubscribe(funcWindow);
    Log::Trace("{0}", glm::rotate(glm::translate(glm::mat4(1.f), glm::vec3(2.f, 3.f, -1.f)), glm::radians(50.f), glm::vec3(0.f, 1.f, 0.f)));

    InitImGui();
}

void Application::InitImGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(Application::GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

glm::ivec2 Application::GetFrameBufferSize()
{
    int width, height;
    glfwGetFramebufferSize(s_Window, &width, &height);
    return { width, height };
}

void Application::Run()
{
    Log::Info("Application starting...");    

    Start();

    while (!glfwWindowShouldClose(s_Window))
    {
        // update the frame buffer
        auto bufferSize = GetFrameBufferSize();
        glViewport(0, 0, bufferSize.x, bufferSize.y);
        glClear(GL_COLOR_BUFFER_BIT);

        // imgui new frame-----
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // --------------------

        Update();
        UpdateImGui();

        // imgui end frame ---------
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = { (float)bufferSize.x, (float)bufferSize.y };

        // Disable and store SRGB state.
        const bool srgbEnabled = glIsEnabled(GL_FRAMEBUFFER_SRGB);
        glDisable(GL_FRAMEBUFFER_SRGB);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (srgbEnabled) glEnable(GL_FRAMEBUFFER_SRGB);

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        // -------------------------

        glfwSwapBuffers(s_Window);
        glfwPollEvents();
    }

    // imgui terminate
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw terminate
    glfwDestroyWindow(s_Window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

static GLuint vertex_buffer, vertex_shader, fragment_shader, program;
static GLint mvp_location, vpos_location, vcol_location;

void Application::Start()
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
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void *)0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void *)(sizeof(float) * 2));
}

void Application::Update()
{
    auto bufferSize = GetFrameBufferSize();
    auto ratio = bufferSize.x/ (float)bufferSize.y;
    glm::mat4 m, p, mvp;
    m = glm::rotate(glm::mat4(1.f), glm::radians((float)glfwGetTime()) * 10.f, glm::vec3(0.f, 0.f, 1.f));
    p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mvp = m * p;

    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)glm::value_ptr(mvp));
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::UpdateImGui()
{
    static float updates = 0.f;
    static float frameTimes = 0.f;
    updates += 1.f;
    frameTimes += Time::GetDeltaSeconds() * 1000.f;

    ImGui::Begin("Debug info");
    ImGui::Text("Scene info:");
    ImGui::Text("FPS:            %f", Time::GetFPS());
    ImGui::Text("Frame time:     %f ms", Time::GetDeltaSeconds() * 1000.f);
    ImGui::Text("Avg FPS:        %f", updates / Time::GetTimeSeconds());
    ImGui::Text("Avg frame time: %f ms", frameTimes / updates);
    ImGui::End();
}
