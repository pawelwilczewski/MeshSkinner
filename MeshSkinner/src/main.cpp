#include "pch.h"
#include "Main/MainScene.h"

int main(void)
{
    Application::Init(1600, 900, "MeshSkinner");
    Application::scene = MakeUnique<MainScene>();
    Application::Run();
}
