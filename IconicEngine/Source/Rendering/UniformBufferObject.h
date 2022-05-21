#pragma once
#include <Core/Object.h>
#include <glad/glad.h>

class UniformBufferObject : public Object
{
public:
	static UniformBufferObject* Create(Object* NewOuter, unsigned int SizeBytes);

	IMPLEMENT_CONSTRUCTOR(UniformBufferObject, Object);

	void Init() override;
	void Shutdown() override;

	void SetBufferSize(unsigned int SizeBytes);
	void ClearBufferData();

	void Bind(unsigned int Index);
	
	unsigned int GetUBO() const;
	unsigned int GetSize() const;
	int GetBoundIndex() const;

public:
	template<typename T>
	void BufferData(T* Data, unsigned int Offset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, Offset, sizeof(T), Data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

protected:
	virtual void BufferBound(unsigned int Index);
	virtual void BufferUnbound(unsigned int Index);
	
private:
	unsigned int UBO;
	unsigned int BufferSize;
	int BoundIndex;

	friend class RenderManager;
};
