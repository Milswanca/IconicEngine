#pragma once
#include <ctime>

#include "IndexArray.h"
#include "LookUpTable.h"
#include "Object.h"
#include "World.h"


class Shader;
struct GLFWwindow;
class Application;
class AssetManager;
class Object;

const static unsigned short INVALID_OBJECT_ID = 65535;
const static unsigned short MAX_OBJECTS = 65534;

class Engine
{
public:
	Engine();
	virtual ~Engine();
	
	static Engine* Get();
	static Shader* BaseShader;
	
	virtual int Init();
	virtual void Run();
	virtual int Shutdown();

	virtual void Update();

	GLFWwindow* GetWindow() const;
	AssetManager* GetAssetManager() const;
	Application* GetApplication() const;
	World* GetActiveWorld() const;
	
	template<typename T>
	T* CreateObject(Object* Outer)
	{
		unsigned short ObjectID = ObjectIDs->Add();
		
		T* Object = new T(Outer == nullptr ? Outer : _World);
		Object->UniqueID = ObjectID;
		ObjectTable->Add(Object);
		Object->Init();

		return Object;
	}

	void DestroyObject(Object* Object)
	{
		ObjectTable->Remove(Object);
		ObjectIDs->Remove(Object->GetUniqueID());

		Object->Shutdown();
		Object->UniqueID = INVALID_OBJECT_ID;
		delete Object;
	}

private:
	static Engine* Inst;
	GLFWwindow* _Window;
	Application* _Application;
	AssetManager* _AssetManager;
	World* _World;
	clock_t LastClock;
	
	IndexArray<unsigned short>* ObjectIDs;
	LookUpTable<Object, unsigned short>* ObjectTable;
};

