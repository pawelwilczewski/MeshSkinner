#pragma once

class UserInterface
{
	friend class Application;

public:
	static glm::ivec2 GetViewportSize();

private:
	static void Init();
	static void FrameBegin();
	static void FrameEnd();
	static void Terminate();
};
