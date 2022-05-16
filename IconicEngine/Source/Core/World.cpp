#include "World.h"
#include "Actor.h"

void UpdateActors(Actor* A, unsigned short I, float DT)
{
    A->Update(DT);
}

void World::Init()
{
    Object::Init();

    Actors = new LookUpTable<Actor, unsigned short>(MAX_ACTORS);
}

void World::Shutdown()
{
    Object::Shutdown();

    delete Actors;
}

void World::Update(const float DeltaTime)
{
    Actors->ForEachObject(UpdateActors, DeltaTime);
}

void World::DestroyActor(Actor* Actor)
{
    Actors->Remove(Actor);
    Actor->Destroy();
}
