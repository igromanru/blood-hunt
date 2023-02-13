#include "aimbot.hpp"
#include "../../../engine/engine.hpp"

#include "../../../settings/config/config.hpp"

HWND game = nullptr;
constexpr auto m_pi = 3.14159265358979323846f;

float distance_from_cursor(const engine::structs::vector_t& sp)
{
	POINT p;
	if (GetCursorPos(&p))
	{
		if (!game)
			game = FindWindowA("UnrealWindow", 0);

		ScreenToClient(game, &p);

		return std::sqrtf(std::powf(p.x - sp.x, 2) + std::powf(p.y - sp.y, 2));
	}

	return 0.f;
}

void aim_at(engine::structs::vector_t aim)
{
	static const auto [width, height] = engine::functions::get_screen_size();

	INPUT i = { 0 };
	i.type = INPUT_MOUSE;
	i.mi.dwFlags = MOUSEEVENTF_MOVE;
	i.mi.dx = (aim.x - width / 2) / std::get<int>(features["smoothing"]);
	i.mi.dy = (aim.y - height / 2) / std::get<int>(features["smoothing"]);
	SendInput(1, &i, sizeof(INPUT));

}

engine::types::actor_t get_closest_player()
{
	engine::types::actor_t best_player = { 0 };

	auto thread_safe_gworld = memory::functions::read< engine::types::gworld_t >(engine::values::process_base + engine::offsets::core::gworld);

	if (!thread_safe_gworld.address)
		return best_player;

	auto owning = thread_safe_gworld.get_owning_game_instance();

	if (!owning.address)
		return best_player;

	auto lp = owning.local_players.get_members(owning.address).front();

	if (!lp.address)
		return best_player;

	auto pc = lp.get_player_controller();

	if (!pc.address)
		return best_player;

	auto ps = pc.get_player_state();

	if (!ps.address)
		return best_player;

	auto distance = 10000.f;

	std::unique_lock locker(engine::values::lock);

	for (const auto pair : engine::values::players)
	{
		auto player = pair.first;

		if (!player.address)
			continue;

		auto player_pawn = player.get_pawn();

		if (!player_pawn.address)
			continue;

		auto actor_ps = player_pawn.get_player_state();

		if (!actor_ps.address)
			continue;

		if (std::get<bool>(features["team_check"]) && !engine::functions::compare_team(actor_ps.get_fguid(), ps.get_fguid()))
			continue;

		const auto health = player_pawn.get_health();

		if (health <= 0)
			continue;

		auto root_world_position = engine::functions::get_bone_position(player, engine::structs::bones::head);

		if (!root_world_position.has_value())
		{
			continue;
		}

		engine::structs::vector_t root_screen_position;

		if (engine::functions::world_to_screen(root_world_position.value(), root_screen_position))
		{
			static const auto [width, height] = engine::functions::get_screen_size();

			auto dist = std::sqrtf(std::powf(root_screen_position.x - width / 2, 2) + std::powf(root_screen_position.y - height / 2, 2));

			if (dist < distance)
			{
				distance = dist;
				best_player = player;
			}
		}
	}

	return best_player;
}

void modules::players::aimbot::callback()
{

	if (!std::get<bool>(features["aimbot"]) || !GetAsyncKeyState(VK_RBUTTON) || (GetForegroundWindow() != engine::values::game_handle && GetForegroundWindow() != FindWindowA("UnrealWindow", nullptr)))
		return;

	auto player = get_closest_player();

	if (!player.address)
		return;

	auto head_world_position = engine::functions::get_bone_position(player, engine::structs::bones::head);

	if (!head_world_position.has_value())
	{
		return;
	}

	engine::structs::vector_t head_screen_position;

	if (engine::functions::world_to_screen(head_world_position.value(), head_screen_position))
	{
		static const auto [width, height] = engine::functions::get_screen_size();

		auto dist = std::sqrtf(std::powf(head_screen_position.x - width / 2, 2) + std::powf(head_screen_position.y - height / 2, 2));

		if (dist < std::get<int>(features["aimbot_fov"]) * 50.f)
			aim_at(head_screen_position);
	}
}
