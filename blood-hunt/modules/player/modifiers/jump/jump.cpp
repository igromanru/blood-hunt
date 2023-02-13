#include "jump.hpp"
#include "../../../../settings/config/config.hpp"

static float old = 0.f;

void modules::players::modifiers::jump::callback(engine::types::player_t local_player)
{
	auto pc = local_player.get_player_controller();

	if (!pc.address)
		return;

	auto pawn = pc.get_pawn();

	if (!pawn.address)
		return;

	auto movement = memory::functions::read<std::uintptr_t>(pawn.address + 0x290);

	if (!movement)
		return;

	//memory::functions::write<float>(movement + 0x18C, 99999.f);

	//memory::functions::write<float>(movement + 0x154, 100.f);

	if (old == 0.f)
		old = memory::functions::read<float>(pawn.address + engine::offsets::pawn::jump_max_hold_time);

	if (std::get<bool>(features["slidefly"]))
		memory::functions::write<float>(pawn.address + engine::offsets::pawn::jump_max_hold_time, 99999.f);
	else
		memory::functions::write<float>(pawn.address + engine::offsets::pawn::jump_max_hold_time, old);
	//memory::functions::write<float>(pawn.address + 0x338, 99999.f);
	//memory::functions::write<float>(pawn.address + 0x334, 0.f);

	//memory::functions::write<int32_t>(pawn.address + 0x344, 99999);
	//memory::functions::write<int32_t>(pawn.address + 0x348, 0);
}