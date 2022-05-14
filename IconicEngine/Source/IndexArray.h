#pragma once

template<typename T>
struct IndexArray
{
public:
    IndexArray(T Length)
    {
        NextFree = new T[Length];
        MaxSize = Length;
        Reset();
    }
    
    ~IndexArray()
    {
        delete[] NextFree;
        NextFree = nullptr;
    }
    
    T Add()
    {
        if(IsFull())
            return -1;

        auto Result = ArrayNext;
        ArrayNext = NextFree[Result];
        NumUsedElements += 1;
        return Result;
    }
    
    void Remove(T Index)
    {
        if(Index < MaxSize)
        {
            NextFree[Index] = ArrayNext;
            ArrayNext = Index;
            NumUsedElements -= 1;
        }
    }
    
    void Reset()
    {
        for (T i = 0; i < MaxSize; ++i)
            NextFree[i] = i + 1;
	
        ArrayNext = 0;
        NumUsedElements = 0;
    }
    
    bool IsFull() const
    {
        return NumUsedElements >= MaxSize;
    }
    
    T Size() const
    {
        return MaxSize;
    }
    
private:
    T* NextFree;

    T ArrayNext;
    T NumUsedElements;
    T MaxSize;
};
