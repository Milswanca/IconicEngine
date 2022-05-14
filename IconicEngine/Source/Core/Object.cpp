#include "Object.h"
#include "Engine.h"

Object::Object(Object* NewOuter)
{
    Outer = NewOuter;
    UniqueID = INVALID_OBJECT_ID;
}

Object::~Object()
{
    
}

Object* Object::GetOuter() const
{
    return Outer;
}

void Object::Init()
{
    
}

void Object::Shutdown()
{
}

void Object::Destroy()
{
    Engine::Get()->DestroyObject(this);
}

World* Object::GetWorld() const
{
    return nullptr;
}

unsigned short Object::GetUniqueID() const
{
    return UniqueID;
}