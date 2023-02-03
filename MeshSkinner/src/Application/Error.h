namespace Error
{
#if defined DEBUG || defined RELEASE
	void GLAPIENTRY CallbackGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
	{
		std::string_view sev;
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION:
#if MIN_SEVERITY_LEVEL == GL_DEBUG_SEVERITY_LOW || MIN_SEVERITY_LEVEL == GL_DEBUG_SEVERITY_MEDIUM || MIN_SEVERITY_LEVEL == GL_DEBUG_SEVERITY_HIGH
			return;
#endif
			sev = "NOTIFICATION";
			break;
		case GL_DEBUG_SEVERITY_LOW:
#if MIN_SEVERITY_LEVEL == GL_DEBUG_SEVERITY_MEDIUM || MIN_SEVERITY_LEVEL == GL_DEBUG_SEVERITY_HIGH
			return;
#endif
			sev = "LOW";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
#if MIN_SEVERITY_LEVEL == GL_DEBUG_SEVERITY_HIGH
			return;
#endif
			sev = "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			sev = "HIGH";
			break;
		default:
			sev = "UNKNOWN";
			break;
		}

		std::string_view errType;
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			errType = "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			errType = "DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			errType = "UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			errType = "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			errType = "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_OTHER:
			errType = "OTHER";
			break;
		case GL_DEBUG_TYPE_MARKER:
			errType = "MARKER";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			errType = "PUSH_GROUP";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			errType = "POP_GROUP";
			break;
		default:
			errType = "UNKNOWN";
			break;
		}

		std::string_view src;
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			src = "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			src = "WINDOW_SYSTEM";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			src = "SHADER_COMPILER";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			src = "THIRD_PARTY";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			src = "APPLICATION";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			src = "OTHER";
			break;
		default:
			src = "UNKNOWN";
			break;
		}

		Log::Error("GL CALLBACK: src = {0}, severity = {1}, type = {2}, id = 0x{3:x}, message = {4}", src, sev, errType, id, message);
	}
#endif
}
