#include "pch.h"
#include "Main/MainScene.h"

int main(void)
{
    Application::Init(1600, 900, "MeshSkinner");
    Application::s_Scene = MakeUnique<MainScene>();
    Application::Run();
}
