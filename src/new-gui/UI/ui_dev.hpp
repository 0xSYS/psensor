#ifndef UI_DEV_HPP
#define UI_DEV_HPP

#include "imgui/imgui.h"

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void RenderFullDemo();
void RenderSimpleDemo();
void RenderNewDemo();
#endif