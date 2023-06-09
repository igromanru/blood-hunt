#include "../oversee.hpp"

#include "../libs/imgui_tricks.hpp"

void OverseeUI::Elements::Separator()
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize({ 328, 5 }, 328, 3);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, 0))
        return;

    window->DrawList->AddLine(bb.Min, bb.Min + ImVec2(328, 0), ImColor(228, 228, 230, 23), 1);
}
