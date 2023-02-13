#include "antiaim.hpp"
#include "../../../engine/engine.hpp"

#include "../../../settings/config/config.hpp"

float to_write = 45.f;

void modules::players::antiaim::callback()
{
	if (!std::get<bool>(features["antiaim"]))
		return;

	auto thread_safe_gworld = memory::functions::read< engine::types::gworld_t >(engine::values::process_base + engine::offsets::core::gworld);

	if (!thread_safe_gworld.address)
		return;

	auto owning_game_instance = thread_safe_gworld.get_owning_game_instance();

	if (!owning_game_instance)
		return;

	auto lp = owning_game_instance.local_players.get_members(owning_game_instance.address).front();

	if (!lp)
		return;

	//std::printf("%p\n", driver::read_virtual_memory<std::uintptr_t>(lp.address) - memory::values::process_base);

	auto pc = lp.get_player_controller();

	if (!pc)
		return;

	to_write = -to_write;

	memory::functions::write<float>(pc.address + 0x428 + 0x4, to_write);
}
