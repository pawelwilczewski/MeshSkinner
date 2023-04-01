#include "pch.h"
#include "MeshSkinner/MainScene.h"

int main(void)
{
    Application::Init({ 1600, 900 }, "MeshSkinner", 0);
    auto scene = MakeUnique<MainScene>();
    Application::Run();
}
