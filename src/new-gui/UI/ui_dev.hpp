#ifndef UI_DEV_HPP
#define UI_DEV_HPP

#include <imgui/imgui.h>

inline bool show_demo_window = false;
inline bool show_implot_demo = false;
static bool show_another_window = false;
inline bool show_full_demo = false;
inline ImVec4 clear_color = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);

void RenderFullDemo();
void RenderSimpleDemo();
void RenderNewDemo();
#endif