#include "pch.h"
#include "Application.h"
#include "Error.h"

Application::Application()
{
	Log::Init();
}

void Application::SetupDebug()
{
	// enable gl debug messages
#if defined DEBUG || defined RELEASE
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(Error::CallbackGL, nullptr);
#endif
}
