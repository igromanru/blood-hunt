#include "modifiers.hpp"

#include "jump/jump.hpp"

void modules::players::modifiers::callback()
{
	auto thread_safe_gworld = memory::functions::read< engine::types::gworld_t >(engine::values::process_base + engine::offsets::core::gworld);

	if (!thread_safe_gworld.address)
		return;

	auto owning_game_instance = thread_safe_gworld.get_owning_game_instance();

	if (!owning_game_instance)
		return;

	auto lp = owning_game_instance.local_players.get_members(owning_game_instance.address).front();

	if (!lp)
		return;

	jump::callback(lp);
}