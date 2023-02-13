#include "menu.hpp"

#include "../renderer.hpp"
#include "../../modules/visuals/esp/esp.hpp"

#include "../../settings/config/config.hpp"

void renderer::menu::draw()
{
	modules::visuals::esp::callback( );

	if ( renderer::values::renderer->is_menu_open )
	{
		ImGui::SetNextWindowSize( ImVec2( 700, 375 ), ImGuiCond_Once );

		ImGui::Begin( "mutex.club" );

		ImGui::Checkbox("box esp", &std::get<bool>(features["esp"]));

		ImGui::Checkbox("health esp", &std::get<bool>(features["health_esp"]));

		ImGui::Checkbox("tracers", &std::get<bool>(features["tracers"]));

		ImGui::Checkbox("distance", &std::get<bool>(features["distance"]));

		ImGui::Checkbox("draw fov", &std::get<bool>(features["draw_fov"]));

		ImGui::Checkbox("draw name", &std::get<bool>(features["draw_name"]));

		ImGui::Separator();

		ImGui::Checkbox("aimbot", &std::get<bool>(features["aimbot"]));

		ImGui::SliderInt("smoothing", &std::get<int>(features["smoothing"]), 1, 20);

		ImGui::SliderInt("aimbot fov", &std::get<int>(features["aimbot_fov"]), 1, 20);

		ImGui::Separator();

		ImGui::Checkbox("antiaim", &std::get<bool>(features["antiaim"]));

		ImGui::End( );

	}
}
