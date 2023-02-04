#pragma once

class Application
{
public:
	static void Init(uint32_t width, uint32_t height, const char *title);
	static void Run();
	static void Terminate();

private:
	static void Start();
	static void EarlyUpdate();
	static void Update();
	static void UpdateUI();
	static void LateUpdate();
};
