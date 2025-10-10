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