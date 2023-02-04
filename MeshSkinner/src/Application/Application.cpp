#include "pch.h"
#include "Application.h"
#include "Error.h"

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

static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void a(int x)
{
    Log::Info("{0}", x);
}
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
    glfwSetKeyCallback(s_Window, KeyCallback);

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
    auto func = MakeCallbackRef<int>([&](int key) { Log::Info("{0}", key); });
    auto funcWindow = MakeCallbackRef<glm::ivec2>([&](glm::ivec2 windowSize) { Log::Info("{0}, {1}", windowSize.x, windowSize.y); });
    Input::OnKeyPressedSubscribe(func);
    //Input::OnKeyPressedUnsubscribe(func);
    Input::OnWindowResizedSubscribe(funcWindow);
}

void Application::Run()
{
    Log::Info("Application starting...");

    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

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

    while (!glfwWindowShouldClose(s_Window))
    {
        float ratio;
        int width, height;
        glm::mat4 m, p, mvp;

        glfwGetFramebufferSize(s_Window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        m = glm::rotate(glm::mat4(1.f), glm::radians((float)glfwGetTime()) * 10.f, glm::vec3(0.f, 0.f, 1.f));
        p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mvp = m * p;

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)glm::value_ptr(mvp));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(s_Window);
        glfwPollEvents();
    }

    glfwDestroyWindow(s_Window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
