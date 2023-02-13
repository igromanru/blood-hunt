#include "../../dependencies/imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../dependencies/imgui/imgui_internal.h"

#include <functional>

#include "font/faprolight.hpp"
#include "font/hashes.h"
#include "font/sffont.hpp"

namespace OverseeUI {

    enum ButtonFlags {
        BUTTON_PURPLE,
        BUTTON_GREEN,
        BUTTON_GRAY
    };

    namespace Elements
    {
        extern void Tab(const char* label, int& tab, int index, std::function<void()> cb = nullptr);

        extern void BeginChild(const char* str_id, const ImVec2 size_arg, bool border = false, ImGuiWindowFlags extra_flags = NULL);
        extern void EndChild();

        extern void Checkbox(const char* label, const char* desc, bool* v);
        extern void Separator();

        extern bool Hotkey(const char* label, int* k);
        extern bool HotkeyStretched(const char* label, int* k);

        extern void Information(const char* label, const char* desc, float p_data, const char* format);
        extern void Information(const char* label, const char* desc, int p_data, const char* format);
        extern void Information(const char* label, const char* desc, const char* p_data, const char* icon);
        extern void Information(const char* label, const char* desc);

        extern bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float power);
        extern bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format);
        extern bool SliderFloatStretched(const char* label, float* v, float v_min, float v_max, const char* format, float power);
        extern bool SliderIntStretched(const char* label, int* v, int v_min, int v_max, const char* format);

        extern bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
        extern void EndCombo();

        extern bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
        extern bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
        extern bool Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

        extern void ColorPicker(const char* identifier, float* col);
        extern bool Button(const char* label, ImVec2 size_arg, OverseeUI::ButtonFlags flag);
        extern bool ConfigButton(const char* label, const char* data, const char* author);

        extern bool Tab(const char* label, std::u8string icon, int& tab, int index);
        extern bool SubtabStatic(const char* label, const char* icon, int& tab, int index);
    }

    namespace Fonts {
        inline ImFont* DrukWideBold;
        inline ImFont* px18;
        inline ImFont* px24;


    }

    namespace Static {
        inline std::string current_tab = "LEGIT";
    }

    namespace Render {

        extern void SetStyles(void*);
        extern void RenderMenu();
    }
}
