#pragma once
#include "Utils/LookUpTable.h"
#include "Object.h"

class Actor;
static const unsigned short MAX_ACTORS = 65534;

class World : public Object
{
public:
    IMPLEMENT_CONSTRUCTOR(World, Object);
    
    virtual void Init() override;
    virtual void Shutdown() override;
    
    virtual void Update(const float DeltaTime);

    template<typename T>
    T* SpawnActor()
    {
        T* Actor = Engine::Get()->CreateObject<T>(this);
        Actors->Add(Actor);
        
        return Actor;
    }

    void DestroyActor(Actor* Actor);

private:
    LookUpTable<Actor, unsigned short>* Actors;
};
