#pragma once

class UserInterface
{
	friend class Application;

private:
	static void Init();
	static void FrameBegin();
	static void FrameEnd();
	static void Terminate();
};
