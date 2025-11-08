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








#ifndef UI_DEV_HPP
#define UI_DEV_HPP

#include <imgui/imgui.h>

inline bool show_demo_window = false;
inline bool show_implot_demo = false;
static bool show_another_window = false;
inline bool show_full_demo = false;

void RenderFullDemo();
void RenderSimpleDemo();
void RenderNewDemo();
#endif