#include "esp.hpp"

#include "../../../engine/engine.hpp"

#include "../../../renderer/renderer.hpp"
#include "../../../settings/config/config.hpp"

void modules::visuals::esp::callback( )
{
	//engine::functions::initiate( );

	auto thread_safe_gworld = memory::functions::read< engine::types::gworld_t >(engine::values::process_base + engine::offsets::core::gworld);

	if (!thread_safe_gworld.address)
		return;

	auto owning = thread_safe_gworld.get_owning_game_instance();

	if (!owning.address)
		return;

	auto lp = owning.local_players.get_members(owning.address).front();

	if (!lp.address)
		return;

	auto pc = lp.get_player_controller();

	if (!pc.address)
		return;

	auto ps = pc.get_player_state();

	if (!ps.address)
		return;

	auto cm = pc.get_camera_manager();

	if (!cm.address)
		return;

	auto position = cm.get_position();

	const auto draw_list = ImGui::GetBackgroundDrawList();

	const auto [w, h] = engine::functions::get_screen_size();

	if (std::get<bool>(features["draw_fov"]))
	{
		auto colorf = std::get<engine::structs::vector4_t>(features["fov_color"]).col;

		draw_list->AddCircle({ w / 2.0f, h / 2.0f }, std::get<int>(features["aimbot_fov"]) * 50, ImColor{ colorf[0],colorf[1],colorf[2],colorf[3] });
	}

	std::unique_lock locker(engine::values::lock);

	for (const auto pair : engine::values::players)
	{
		 auto player = pair.first;

		if ( !player.address )
			continue;

		auto rc = player.get_root_component();

		if (!rc.address)
			continue;

		const auto delta = rc.get_component_to_world().translation - position;

		const auto distance = static_cast<std::uint32_t>(sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) / 10);

		auto head_world_position = engine::functions::get_bone_position( player, engine::structs::bones::head );
		auto root_world_position = engine::functions::get_bone_position( player, engine::structs::bones::root );

		if (!head_world_position.has_value() || !root_world_position.has_value())
		{
			continue;
		}

		engine::structs::vector_t head_screen_position, root_screen_position;

		if ( engine::functions::world_to_screen( head_world_position.value(), head_screen_position ) && engine::functions::world_to_screen( root_world_position.value(), root_screen_position ) )
		{

			auto player_pawn = player.get_pawn();

			if (!player_pawn.address)
				continue;

			auto actor_ps = player_pawn.get_player_state();

			if (!actor_ps.address)
				continue;

			if (std::get<bool>(features["team_check"]) && !engine::functions::compare_team(actor_ps.get_fguid(), ps.get_fguid()))
				continue;

			const auto health = player_pawn.get_health( );

			if ( distance > 1500 || health <= 0)
				continue;

			const float width = fabs(head_screen_position.y - root_screen_position.y) / 4.0f;
			const float height = fabs(head_screen_position.y - root_screen_position.y) * 1.2f;

			const auto line_len_width = width / 2.5f;

			if (std::get<bool>(features["esp"]))
			{
				auto colorf = std::get<engine::structs::vector4_t>(features["esp_color"]).col;

				auto color = ImColor{ colorf[0],colorf[1],colorf[2],colorf[3] };

				draw_list->AddLine({ root_screen_position.x - width, root_screen_position.y - height }, { root_screen_position.x - width + line_len_width, root_screen_position.y - height }, color);
				draw_list->AddLine({ root_screen_position.x - width, root_screen_position.y - height }, { root_screen_position.x - width, (root_screen_position.y - height) + line_len_width }, color); //top left

				draw_list->AddLine({ root_screen_position.x + width, root_screen_position.y - height }, { root_screen_position.x + width - line_len_width, root_screen_position.y - height }, color);
				draw_list->AddLine({ root_screen_position.x + width, root_screen_position.y - height }, { root_screen_position.x + width, (root_screen_position.y - height) + line_len_width }, color); //top right

				draw_list->AddLine({ root_screen_position.x - width, root_screen_position.y }, { root_screen_position.x - width + line_len_width, root_screen_position.y }, color); //bottom left
				draw_list->AddLine({ root_screen_position.x - width, root_screen_position.y }, { root_screen_position.x - width, root_screen_position.y - line_len_width }, color);

				draw_list->AddLine({ root_screen_position.x + width, root_screen_position.y }, { root_screen_position.x + width - line_len_width, root_screen_position.y }, color); //bottom right
				draw_list->AddLine({ root_screen_position.x + width, root_screen_position.y }, { root_screen_position.x + width, root_screen_position.y - line_len_width }, color);

				//border 

				draw_list->AddLine({ root_screen_position.x - width + 1, root_screen_position.y - height + 1 }, { root_screen_position.x - width + line_len_width, root_screen_position.y - height + 1 }, ImColor{ 0,0,0 });
				draw_list->AddLine({ root_screen_position.x - width + 1, root_screen_position.y - height + 1 }, { root_screen_position.x - width + 1, (root_screen_position.y - height) + line_len_width }, ImColor{ 0,0,0 }); //top left

				draw_list->AddLine({ root_screen_position.x + width - 1, root_screen_position.y - height + 1 }, { root_screen_position.x + width - line_len_width, root_screen_position.y - height + 1 }, ImColor{ 0,0,0 });
				draw_list->AddLine({ root_screen_position.x + width - 1, root_screen_position.y - height + 1 }, { root_screen_position.x + width - 1, (root_screen_position.y - height) + line_len_width }, ImColor{ 0,0,0 }); //top right

				draw_list->AddLine({ root_screen_position.x - width + 1, root_screen_position.y - 1 }, { root_screen_position.x - width + line_len_width, root_screen_position.y - 1 }, ImColor{ 0,0,0 }); //bottom left
				draw_list->AddLine({ root_screen_position.x - width + 1, root_screen_position.y - 1 }, { root_screen_position.x - width + 1, root_screen_position.y - line_len_width - 1 }, ImColor{ 0,0,0 });

				draw_list->AddLine({ root_screen_position.x + width - 1, root_screen_position.y - 1 }, { root_screen_position.x + width - line_len_width, root_screen_position.y - 1 }, ImColor{ 0,0,0 }); //bottom right
				draw_list->AddLine({ root_screen_position.x + width - 1, root_screen_position.y - 1 }, { root_screen_position.x + width - 1, root_screen_position.y - line_len_width - 1 }, ImColor{ 0,0,0 });
			}

			if (std::get<bool>(features["distance"]))
			{
				auto colorf = std::get<engine::structs::vector4_t>(features["distance_color"]).col;

				std::string to_draw = "[";
				to_draw.append(std::to_string(distance));
				to_draw.append("m]");

				draw_list->AddText({ root_screen_position.x - width, root_screen_position.y + 10.f }, ImColor{ colorf[0],colorf[1],colorf[2],colorf[3] }, to_draw.c_str());
			}

			if (std::get<bool>(features["health_esp"]))
			{
				auto colorf = std::get<engine::structs::vector4_t>(features["health_color"]).col;

				auto color = ImColor{ colorf[0],colorf[1],colorf[2],colorf[3] };

				const auto new_height_health = (height) / player_pawn.get_max_health() * health;

				std::printf("health %f\n", health);

				draw_list->AddRectFilled({ root_screen_position.x + width + (width / 10.f) / 4.5f, root_screen_position.y - 2 }, { root_screen_position.x + width + (width / 10.f), root_screen_position.y - new_height_health }, color);
				draw_list->AddRect({ root_screen_position.x + width + (width / 10.f) / 4.5f - 1, root_screen_position.y - 1 }, { root_screen_position.x + width + (width / 10.f) + 1, root_screen_position.y - new_height_health - 1 }, ImColor{ 1, 1, 1 });
			}

			if (std::get<bool>(features["tracers"]))
			{
				auto colorf = std::get<engine::structs::vector4_t>(features["tracers_color"]).col;

				draw_list->AddLine({ w / 2.0f, static_cast<float>(h) }, { root_screen_position.x, root_screen_position.y }, ImColor{ colorf[0],colorf[1],colorf[2],colorf[3] });
			}

			if (std::get<bool>(features["draw_name"]))
			{
				auto colorf = std::get<engine::structs::vector4_t>(features["name_color"]).col;

				const auto name = pair.second;

				draw_list->AddText({ root_screen_position.x - ImGui::CalcTextSize(name.c_str()).x / 2 , root_screen_position.y - height - 20.f }, ImColor{ colorf[0],colorf[1],colorf[2],colorf[3] }, name.c_str());
			}
		}
	}
}
