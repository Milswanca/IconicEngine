#pragma once
#include <Core/Object.h>
#include <glad/glad.h>

class UniformBufferObject : public Object
{
public:
	IMPLEMENT_CONSTRUCTOR(UniformBufferObject, Object);

	void Init() override;
	void Shutdown() override;

	template<typename T>
	void BufferData(T* Data)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(T), Data, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		BufferSize = sizeof(T);
	}
	
	void Bind(unsigned int Index);
	
	unsigned int GetUBO() const;
	unsigned int GetSize() const;
	int GetBoundIndex() const;

protected:
	virtual void BufferBound(unsigned int Index);
	virtual void BufferUnbound(unsigned int Index);
	
private:
	unsigned int UBO;
	unsigned int BufferSize;
	int BoundIndex;

	friend class RenderManager;
};
