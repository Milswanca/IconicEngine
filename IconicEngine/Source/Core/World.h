#pragma once
#include "LookUpTable.h"
#include "Object.h"

class Actor;
static const unsigned short MAX_ACTORS = 65534;

class World : public Object
{
public:
    World(Object* NewOuter);
    ~World();
    
    virtual void Update(const float DeltaTime);

    template<typename T>
    T* SpawnActor()
    {
        T* Actor = Engine::Get()->CreateObject<T>(this);
        Actors->Add(Actor);
        Actor->Spawned();
        
        return Actor;
    }

    void DestroyActor(Actor* Actor);

private:
    LookUpTable<Actor, unsigned short>* Actors;
};
