#pragma once
class World;

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

protected:
    virtual void Init();
    virtual void Shutdown();
    
private:
    Object* Outer;
    unsigned short UniqueID;
    
    friend class Engine;
};
