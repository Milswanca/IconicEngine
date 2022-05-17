#pragma once

class Shader;

class IDrawable
{
public:
    virtual void Draw(Shader* Shad) = 0;
};
