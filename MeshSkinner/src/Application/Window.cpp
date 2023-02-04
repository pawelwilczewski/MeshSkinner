#include "pch.h"
#include "Window.h"
#include "Error.h"

GLFWwindow *Window::s_Window;
GLuint Window::s_FramebufferTexture;

static GLuint fbo, rbo;

void Window::Init(int width, int height, const char *title, int vsync)
{
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

    glfwSwapInterval(vsync);

    // --------------------------
    // setup rendering to texture

    // create the FBO
    glGenFramebuffers(1, &fbo); // TODO: change glGen... to glCreate...
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // create the render target texture
    glGenTextures(1, &s_FramebufferTexture);
    glBindTexture(GL_TEXTURE_2D, s_FramebufferTexture);
    auto bufferSize = Window::GetFramebufferSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferSize.x, bufferSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_FramebufferTexture, 0);

    // create the RBO
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferSize.x, bufferSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // check if the FBO is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log::Error("Framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::FrameBegin()
{
    // clear the frame buffer
    auto bufferSize = Window::GetFramebufferSize();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, bufferSize.x, bufferSize.y);
    glClear(GL_COLOR_BUFFER_BIT);

    // setup rendering to the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::FrameEnd()
{
    // swap buffers poll events
    glfwSwapBuffers(Window::GetNativeWindow());
    glfwPollEvents();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::Terminate()
{
    glfwDestroyWindow(s_Window);
    glfwTerminate();
}
