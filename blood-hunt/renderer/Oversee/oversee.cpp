#include "oversee.hpp"

#include "font/DrukWideBold.hpp"
#include "font/Poppins.hpp"
#include "libs/imgui_tricks.hpp"

#include "rawdata/person.hpp"
#include "rawdata/skeleton.hpp"
#include "../../settings/config/config.hpp"

void OverseeUI::Render::SetStyles(void*)
{
    ImGui::GetStyle().WindowPadding = { 0,0 };
    ImGui::GetStyle().WindowBorderSize = 0;
    ImGui::GetStyle().PopupBorderSize = 0;
    ImGui::GetStyle().PopupRounding = 5;
    ImGui::GetStyle().WindowRounding = 16;
    ImGui::GetStyle().ScrollbarSize = 4;

    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImColor{ 18, 18, 18 };
    ImGui::GetStyle().Colors[ImGuiCol_PopupBg] = ImColor{ 33, 33, 33 };
    ImGui::GetStyle().Colors[ImGuiCol_Text] = ImColor{ 119, 119, 119 };
    ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] = ImColor{ 119, 119, 119, 0 };

    ImGui::GetStyle().Colors[ImGuiCol_Header] = ImColor{ 35, 42, 51 };
    ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = ImColor{ 35, 42, 51 };
    ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = ImColor{ 35, 42, 51 };

    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    static const ImWchar ranges[] = {
        0x0020, 0x00FF,0x2000, 0x206F,0x3000, 0x30FF,0x31F0, 0x31FF, 0xFF00,
        0xFFEF,0x4e00, 0x9FAF,0x0400, 0x052F,0x2DE0, 0x2DFF,0xA640, 0xA69F, 0
    };

    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;

    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Poppins_compressed_data, Poppins_compressed_size, 14, NULL, ranges);
    ImGui::GetIO().Fonts->AddFontFromMemoryTTF(faprolight, sizeof faprolight, 18, &icons_config, icon_ranges);

    Fonts::DrukWideBold = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(DrukWideBold_compressed_data, DrukWideBold_compressed_size, 30, NULL, ranges);

    Fonts::px18 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Poppins_compressed_data, Poppins_compressed_size, 18, NULL, ranges);
    Fonts::px24 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Poppins_compressed_data, Poppins_compressed_size, 24, NULL, ranges);
}

void OverseeUI::Render::RenderMenu() {

    auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowSize({ 375, 770 });

    ImGui::Begin("Interface", nullptr, flags);
    {
        auto draw = ImGui::GetWindowDrawList();
        auto pos = ImGui::GetWindowPos();

        draw->AddText(Fonts::DrukWideBold, 30, pos + ImVec2(24, 36), ImColor(255, 255, 255), "mutex");
        draw->AddText(Fonts::DrukWideBold, 30, pos + ImVec2(130, 36), ImColor(127, 128, 246), ".");
        draw->AddText(Fonts::DrukWideBold, 30, pos + ImVec2(140, 36), ImColor(255, 255, 255), "club");

        draw->AddRectFilled(pos + ImVec2(24, 73), pos + ImVec2(24 + 187, 73 + 2), ImColor(255, 255, 255, 10));
        draw->AddRectFilled(pos + ImVec2(24, 73), pos + ImVec2(24 + 88, 73 + 2), ImColor(127, 128, 246));

        static int tab = 0;

        ImGui::SetCursorPos({ 24, 107 });
        ImGui::BeginGroup();
        {
            Elements::Tab("LEGIT", ICON_FA_SHIELD_CHECK, tab, 0);
            ImGui::SameLine();
            Elements::Tab("RAGE", ICON_FA_SHIELD, tab, 1);
            ImGui::SameLine();
            Elements::Tab("MISC", ICON_FA_FOLDER, tab, 2);
            ImGui::SameLine();
            Elements::Tab("VISUALS", ICON_FA_BOLT, tab, 3);
            ImGui::SameLine();
            Elements::Tab("CFG", ICON_FA_ARCHIVE, tab, 4);

        }
        ImGui::EndGroup();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });

        ImGui::SetCursorPos({ 24, 235 });
        ImGui::BeginChild("Main", { 337, 530 }, false, ImGuiWindowFlags_NoScrollbar);
        {
            if (tab == 0)
            {
                Elements::Checkbox("Aimbot", "Subname", &std::get<bool>(features["aimbot"]));
                Elements::Separator();

                Elements::Information("Smooth", "Subname", std::get<int>(features["smoothing"]), "%i");
                Elements::SliderInt("##1", &std::get<int>(features["smoothing"]), 1, 20, "");
                Elements::Separator();

                Elements::Information("FOV", "Subname", std::get<int>(features["aimbot_fov"]), "%i");
                Elements::SliderInt("##4", &std::get<int>(features["aimbot_fov"]), 1, 20, "");
                Elements::Separator();

                //Elements::Information("Bone", "Subname", slider, "%i");
                //Elements::Combo("##2", &slider, "math\0", 1);
                //Elements::Separator();

                //Elements::Information("Max Distance", "Subname", slider, "%i");
                //Elements::SliderInt("##3", &slider, 0, 100, "");
                //Elements::Separator();

                //Elements::Information("Aimbot", "Subname");
                //Elements::Hotkey("##5", &slider);
            }

            if (tab == 1)
            {
                Elements::Checkbox("Antiaim", "Subname", &std::get<bool>(features["antiaim"]));
                Elements::Separator();

                //static bool checkbox = false;
                //Elements::Checkbox("Silent Aimbot", "Subname", checkbox);
                //Elements::Separator();

                //Elements::Checkbox("Magic bullet", "Subname", checkbox);
                //Elements::Separator();

                //Elements::Checkbox("Trigger Aimbot", "Subname", checkbox);
                //Elements::Separator();
            }

            if (tab == 2)
            {
                Elements::Checkbox("Team Check", "Subname", &std::get<bool>(features["team_check"]));
                Elements::Separator();

                Elements::Checkbox("Slide fly", "Sprint then slide and hold jump to fly", &std::get<bool>(features["slidefly"]));
                Elements::Separator();

                //static bool checkbox = false;
                //static int slider = 0;
                //Elements::Checkbox("No Clip", "Subname", checkbox);
                //Elements::Hotkey("##1", &slider);
                //Elements::Separator();

                //Elements::Checkbox("Shoting Thougt Walls", "Subname", checkbox);
                //Elements::Hotkey("##2", &slider);
                //Elements::Separator();

                //Elements::Checkbox("Invisablity", "Subname", checkbox);
                //Elements::Hotkey("##3", &slider);
                //Elements::Separator();

                //Elements::Checkbox("Spiderman", "Subname", checkbox);
                //Elements::Hotkey("##4", &slider);
                //Elements::Separator();

                //Elements::Checkbox("Speedhack", "Subname", checkbox);
                //Elements::Hotkey("##5", &slider);
                //Elements::Separator();

                //Elements::Checkbox("Teleport", "Subname", checkbox);
                //Elements::Hotkey("##6", &slider);
                //Elements::Separator();

                //Elements::Checkbox("Teleport Bot", "Subname", checkbox);
                //Elements::Hotkey("##7", &slider);
                //Elements::Separator();
            }

            if (tab == 3)
            {
                Elements::Checkbox("Box", "Subname", &std::get<bool>(features["esp"]));
                Elements::ColorPicker("##1", std::get<engine::structs::vector4_t>(features["esp_color"]).col);
                Elements::Separator();

                Elements::Checkbox("Health", "Subname", &std::get<bool>(features["health_esp"]));
                Elements::ColorPicker("##2", std::get<engine::structs::vector4_t>(features["health_color"]).col);
                Elements::Separator();

                Elements::Checkbox("Tracers", "Subname", &std::get<bool>(features["tracers"]));
                Elements::ColorPicker("##3", std::get<engine::structs::vector4_t>(features["tracers_color"]).col);
                Elements::Separator();

                Elements::Checkbox("Distance", "Subname", &std::get<bool>(features["distance"]));
                Elements::ColorPicker("##4", std::get<engine::structs::vector4_t>(features["distance_color"]).col);
                Elements::Separator();

                Elements::Checkbox("Fov", "Subname", &std::get<bool>(features["draw_fov"]));
                Elements::ColorPicker("##5", std::get<engine::structs::vector4_t>(features["fov_color"]).col);
                Elements::Separator();

                Elements::Checkbox("Name", "Subname", &std::get<bool>(features["draw_name"]));
                Elements::ColorPicker("##6", std::get<engine::structs::vector4_t>(features["name_color"]).col);
                Elements::Separator();

                //ImGui::Checkbox("box esp", &std::get<bool>(features["esp"]));

                //ImGui::Checkbox("health esp", &std::get<bool>(features["health_esp"]));

                //ImGui::Checkbox("tracers", &std::get<bool>(features["tracers"]));

                //ImGui::Checkbox("distance", &std::get<bool>(features["distance"]));

                //ImGui::Checkbox("draw fov", &std::get<bool>(features["draw_fov"]));

                //ImGui::Checkbox("draw name", &std::get<bool>(features["draw_name"]));

                //static bool checkbox = false;
                //static float col[4];
                //Elements::Checkbox("Skeleton", "Subname", checkbox);
                //Elements::ColorPicker("##1", col);
                //Elements::Separator();

                //Elements::Checkbox("Nickname", "Subname", checkbox);
                //Elements::ColorPicker("##2", col);
                //Elements::Separator();

                //Elements::Checkbox("Box", "Subname", checkbox);
                //Elements::ColorPicker("##3", col);
                //Elements::Separator();

                //Elements::Checkbox("HP Bar", "Subname", checkbox);
                //Elements::ColorPicker("##4", col);
                //Elements::Separator();

                //Elements::Checkbox("Armor", "Subname", checkbox);
                //Elements::ColorPicker("##5", col);
                //Elements::Separator();

                //Elements::Checkbox("Loot", "Subname", checkbox);
                //Elements::ColorPicker("##6", col);
                //Elements::Separator();
            }
        }
        ImGui::EndChild();

        ImGui::PopStyleVar();
    }
    ImGui::End();
}
