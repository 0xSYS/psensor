/*
 * Copyright (C) 2025 kiptunor
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */








#ifndef IMPLOT_PLOT_HPP
#define IMPLOT_PLOT_HPP

#include <imgui/imgui.h>
#include <implot/implot.h>

#include "settings_utils.hpp"



struct ScrollingBuffer
{
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = liveSettings.scroll_buffer_size)
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