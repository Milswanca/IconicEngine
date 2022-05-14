#pragma once

template<typename ObjectType, typename SizeT>
class LookUpTable
{
public:
    typedef void (*TIterateObjectsFunc)(ObjectType*, SizeT, ...);
    
    LookUpTable(SizeT Size)
    {
        //Add an entry onto the end thats invalid boid index
        LookUpIndices = new SizeT[Size];
        Objects = new ObjectType*[Size];
        MaxTableSize = Size;

        Reset();
    }
    
    ~LookUpTable()
    {
        delete[] LookUpIndices;
        delete[] Objects;
    }
    
    SizeT Add(ObjectType* Object)
    {
        if(IsFull())
            return -1;
    
        SizeT Index = Object->GetUniqueID();
        if (Index != -1)
        {
            if (LookUpIndices[Index] != (SizeT)-1)
            {
                //WARNING HERE
            }
            else
            {
                Objects[LookUpTableSize] = Object;
                LookUpIndices[Index] = LookUpTableSize++;
            }

            return LookUpIndices[Index];
        }

        return -1;
    }
    
    SizeT Remove(ObjectType* Object)
    {
        SizeT Index = Object->GetUniqueID();

        if (Index != -1)
        {
            auto SwapIndex = LookUpIndices[Index];

            if (SwapIndex != -1)
            {
                auto LastObject = Objects[LookUpTableSize - 1];
                auto LastObjectIndex = LastObject->GetUniqueID();

                Objects[SwapIndex] = LastObject;
                LookUpIndices[LastObjectIndex] = SwapIndex;

                LookUpIndices[Index] = -1;
                LookUpTableSize -= 1;

                return SwapIndex;
            }
        }

        return -1;
    }

    SizeT Find(ObjectType* Object)
    {
        return LookUpIndices[Object->GetUniqueID()];
    }
    
    bool Contains(ObjectType* Object)
    {
        return Find(Object) != -1;
    }
    
    bool IsFull() const
    {
        return LookUpTableSize >= MaxTableSize;
    }

    void Reset()
    {
        for(SizeT i = 0; i < MaxTableSize; ++i)
        {
            LookUpIndices[i] = -1;
            Objects[i] = nullptr;
        }

        LookUpTableSize = 0;
    }

    template<typename... Args>
    void ForEachObject(void(*Func)(ObjectType*, SizeT, Args...), Args... Argsv)
    {
        for(SizeT i = 0; i < LookUpTableSize; ++i)
        {
            Func(Objects[i], i, Argsv...);
        }
    }
    
private:
    ObjectType** Objects;
    SizeT* LookUpIndices;
    SizeT LookUpTableSize;

    SizeT MaxTableSize;
};
