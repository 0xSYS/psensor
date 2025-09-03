#ifndef IMPLOT_PLOT_HPP
#define IMPLOT_PLOT_HPP

#include <imgui/imgui.h>
#include <implot/implot.h>



struct ScrollingBuffer
{
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 12000)
    {
        MaxSize = max_size;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y)
    {
        if(Data.size() < MaxSize)
            Data.push_back(ImVec2(x,y));
        else
        {
            Data[Offset] = ImVec2(x,y);
            Offset =  (Offset + 1) % MaxSize;
        }
    }
    void Erase()
    {
        if(Data.size() > 0)
        {
            Data.clear();   // Maybe a better UI performance improvement ?
            Data.shrink(0);
            Offset  = 0;
        }
    }
};
#endif