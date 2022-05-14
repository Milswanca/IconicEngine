#include "World.h"
#include "Actor.h"

World::World(Object* NewOuter) : Object(NewOuter)
{
    Actors = new LookUpTable<Actor, unsigned short>(MAX_ACTORS);
}

World::~World()
{
    delete Actors;
}

void UpdateActors(Actor* A, unsigned short I, float DT)
{
    A->Update(DT);
}

void World::Update(const float DeltaTime)
{
    Actors->ForEachObject(UpdateActors, DeltaTime);
}

void World::DestroyActor(Actor* Actor)
{
    Actor->RemovedFromWorld();
    Actors->Remove(Actor);
    Actor->Destroy();
}
