#pragma once
class AssetManager;
class RenderManager;
class Engine;
class World;

#define IMPLEMENT_CONSTRUCTOR(className, baseClass) \
    className() = delete; \
    className(Object* NewOuter) : baseClass(NewOuter) {}; \
    virtual ~className() {};

class Object
{
public:
    Object() = delete;
    Object(Object* NewOuter);
    virtual ~Object();

    template<typename T>
    T* CreateObject(Object* Outer)
    {
        return Engine::Get()->CreateObject<T>(Outer);
    }
    
    void Destroy();
    virtual World* GetWorld() const;

    Object* GetOuter() const;
    unsigned short GetUniqueID() const;

    Engine* GetEngine() const;
    RenderManager* GetRenderManager();
    AssetManager* GetAssetManager();

protected:
    virtual void Init();
    virtual void Shutdown();
    
private:
    Object* Outer;
    unsigned short UniqueID;
    
    friend class Engine;
};
