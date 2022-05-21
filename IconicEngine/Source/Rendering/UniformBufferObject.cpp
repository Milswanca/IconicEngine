#include "UniformBufferObject.h"
#include "Core/Engine.h"
#include "RenderManager.h"

UniformBufferObject* UniformBufferObject::Create(Object* NewOuter, unsigned int SizeBytes)
{
    UniformBufferObject* Buffer = Engine::Get()->CreateObject<UniformBufferObject>(NewOuter);
    Buffer->SetBufferSize(SizeBytes);
    return Buffer;
}

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

void UniformBufferObject::SetBufferSize(unsigned int SizeBytes)
{
    BufferSize = SizeBytes;

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, SizeBytes, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::ClearBufferData()
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, BufferSize, NULL);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
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