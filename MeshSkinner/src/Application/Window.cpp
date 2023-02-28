#include "pch.h"
#include "Window.h"
#include "Error.h"

GLFWwindow *Window::window;
GLuint Window::framebufferTexture;

static GLuint fbo, rbo;
static glm::ivec2 previousBufferSize = glm::ivec2(0);

void Window::Init(const glm::ivec2 &windowSize, const char *title, int vsync)
{
    glfwSetErrorCallback(Error::CallbackGLFW);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(windowSize.x, windowSize.y, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

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

    // gl setup - TODO: better move elsewhere
    glEnable(GL_DEPTH_TEST);

    // create the framebuffer and framebuffer texture
    glCreateFramebuffers(1, &fbo);
    glCreateTextures(GL_TEXTURE_2D, 1, &framebufferTexture);
    glCreateRenderbuffers(1, &rbo);
}

void Window::FrameBegin()
{
    auto bufferSize = UserInterface::GetViewportSize();

    // regen buffer if window resized
    if (bufferSize != previousBufferSize)
        RegenFramebuffer(bufferSize);
    previousBufferSize = bufferSize;

    // bind framebuffer and clear viewport
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, bufferSize.x, bufferSize.y);
}

void Window::FrameEnd()
{
    // swap buffers poll events
    glfwSwapBuffers(Window::GetNativeWindow());
    glfwPollEvents();
    
    // TODO: abstraction for all the viewport and framebuffer logic
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::Terminate()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::RegenFramebuffer(const glm::ivec2 bufferSize)
{
    Log::Trace("Regen framebuffer {}", bufferSize);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // create the render target texture
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferSize.x, bufferSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    // create the RBO
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferSize.x, bufferSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // check if the FBO is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log::Error("Framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLFWwindow *Window::GetNativeWindow() { return window; }

glm::ivec2 Window::GetFramebufferSize()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return { width, height };
}

glm::ivec2 Window::GetWindowPosition()
{
    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);
    return glm::ivec2(xpos, ypos);
}

GLuint Window::GetFramebufferTexture() { return framebufferTexture; }

bool Window::IsPendingClose() { return glfwWindowShouldClose(window); }

void Window::SetCursorVisibility(bool visible)
{
    glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
