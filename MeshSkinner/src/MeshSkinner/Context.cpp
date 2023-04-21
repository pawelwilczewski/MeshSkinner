#include "pch.h"
#include "Context.h"

Context &Context::Get()
{
    static Context instance;
    return instance;
}

Entity *Context::GetSelectedEntity() const
{
    return selectedEntity;
}

SkeletalMeshComponent *Context::GetSelectedSkeletalMesh() const
{
    return selectedSkeletalMesh;
}

Bone *Context::GetSelectedBone() const
{
    return selectedBone;
}

uint32_t Context::GetSelectedBoneIndex() const
{
    auto &bones = selectedSkeletalMesh->skeleton->GetBones();
    return (uint32_t)(std::find(bones.begin(), bones.end(), GetSelectedBone()) - bones.begin());
}

void Context::UpdateSelection(Entity *entity)
{
    auto bone = dynamic_cast<Bone *>(entity);

    // if selected a bone - update selectedBone and select the skeletal mesh entity instead
    if (bone)
    {
        selectedBone = bone;
        auto p = bone->GetParent();

        while (true)
        {
            auto components = p->GetComponents<SkeletalMeshComponent>();
            if (components.size() > 0)
            {
                selectedEntity = p;
                selectedSkeletalMesh = (*components.begin()).get();
                break;
            }

            p = p->GetParent();
        }
    }
    // selected a non-bone entity
    else
    {
        selectedEntity = entity;
        selectedBone = nullptr;

        if (entity)
        {
            auto components = entity->GetComponents<SkeletalMeshComponent>();
            selectedSkeletalMesh = components.size() > 0 ? (*components.begin()).get() : nullptr;
        }
        else
            selectedSkeletalMesh = nullptr;
    }

    Renderer::selectedEntity = selectedEntity;
    if (selectedEntity && selectedBone)
        Renderer::selectedBone = GetSelectedBoneIndex();
    else
        Renderer::selectedBone = -1;
}

void Context::UpdateSelectedBone(uint32_t boneIndex)
{
    selectedBone = Context::Get().selectedSkeletalMesh->skeleton->GetBones()[boneIndex];
    Renderer::selectedBone = GetSelectedBoneIndex();
}

std::unordered_map<SkeletalMeshComponent *, AnimationInfo> &Context::GetAnimationsRaw()
{
    return animations;
}

void Context::ImportAnimations(const std::string &path, SkeletalMeshComponent *mesh)
{
    MeshLibrary::Import(path, animations[mesh].animations);
}
