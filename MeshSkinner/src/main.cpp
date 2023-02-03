#include "pch.h"

int main(void)
{
    auto app = MakeUnique<Application>(1600, 900, "MeshSkinning");
    app->Run();
}
