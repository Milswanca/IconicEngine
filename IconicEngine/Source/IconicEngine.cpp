#include "Core/Engine.h"

int main()
{
    Engine* Eng = new Engine();

    if(Eng->Init() != 0)
    {
        return -1;
    }

    Eng->Run();

    if(!Eng->Shutdown())
    {
        return -1;
    }

    delete Eng;
    return 0;
}