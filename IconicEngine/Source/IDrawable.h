#pragma once

class Material;

class IDrawable
{
public:
    virtual void Draw(Material* Mat) = 0;
};
