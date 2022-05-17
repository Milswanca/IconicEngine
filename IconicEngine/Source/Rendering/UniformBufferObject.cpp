#include "UniformBufferObject.h"

#include "RenderManager.h"

void UniformBufferObject::Init()
{
    Object::Init();

    glGenBuffers(1, &UBO);
}

void UniformBufferObject::Shutdown()
{
    Object::Shutdown();

    glDeleteBuffers(1, &UBO);
}

void UniformBufferObject::Bind(unsigned Index)
{
    GetRenderManager()->BindUniformBuffer(this, Index);
}

unsigned UniformBufferObject::GetUBO() const
{
    return UBO;
}

unsigned UniformBufferObject::GetSize() const
{
    return BufferSize;
}

int UniformBufferObject::GetBoundIndex() const
{
    return BoundIndex;
}

void UniformBufferObject::BufferBound(unsigned Index)
{
    BoundIndex = Index;
}

void UniformBufferObject::BufferUnbound(unsigned Index)
{
    BoundIndex = -1;
}
