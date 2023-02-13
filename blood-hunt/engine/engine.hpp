#pragma once

#include "structures/structures.hpp"

#include "../memory/memory.hpp"
#include <vector>
#include <optional>
#include <mutex>

namespace engine::offsets::core
{
	constexpr auto gobjects = 0x54c3068;

	constexpr auto fnamearray = 0x54aa8c0;

	constexpr auto gworld = 0x55fcbc0;
}

namespace engine::offsets::player_state
{
	constexpr auto player_name_private = 0x0308;

	constexpr auto group_id = 0x3E0;
}

namespace engine::offsets::ulevel
{
	constexpr auto actors = 0xA8;
}

namespace engine::offsets::ugameinstance
{
	constexpr auto localplayers = 0x38;
}

namespace engine::offsets::gworld
{
	constexpr auto ulevel = 0x30;

	constexpr auto owning_game_instance = 0x180;

	constexpr auto game_state_base = 0x120;
}

namespace engine::offsets::actor
{
	constexpr auto pawn = 0x120;

	constexpr auto root_component = 0x138;
}

namespace engine::offsets::player
{
	constexpr auto player_controller = 0x30;
}

namespace engine::offsets::player_controller
{
	constexpr auto character = 0x268;

	constexpr auto pawn = 0x258;

	constexpr auto is_local_player = 0x55C;

	constexpr auto player_state = 0x230;
}

namespace engine::offsets::character
{
	constexpr auto camera_manager = 0x2C0;
}

namespace engine::offsets::camera_manager
{
	constexpr auto camera_cache_private = 0x1AB0;

	constexpr auto pov = 0x10;

	constexpr auto location = 0x0;

	constexpr auto rotation = 0xC;

	constexpr auto fov = 0x18;
}

namespace engine::offsets::pawn
{
	constexpr auto mesh = 0x288;

	constexpr auto health = 0x668;

	constexpr auto max_health = 0x56C;

	constexpr auto player_controller = 0x260;

	constexpr auto ranged_weapon = 0x4c8;

	constexpr auto player_state = 0x248;

	constexpr auto shield = 0x6a2;

	constexpr auto max_shield = 0x6a0;

	constexpr auto jump_max_hold_time = 0x340;
}

namespace engine::offsets::ranged_weapon
{
	constexpr auto is_aiming = 0x1ec;

	constexpr auto aimed_fov = 0x18c;
}

namespace engine::offsets::mesh
{
	constexpr auto cached_bone_space = 0x750;

	constexpr auto component_to_world = 0x1C0;
}

namespace engine::types
{
	struct gworld_t;
	struct actor_t;
	struct fnames_t;
	struct player_t;
	struct player_controller_t;
}

namespace engine::values
{
	extern std::atomic < types::gworld_t > gworld;
	extern types::fnames_t fnames;
	extern std::atomic < std::uintptr_t > process_base;
	extern HWND game_handle;

	extern std::mutex lock;
	extern std::vector<std::pair<engine::types::actor_t, std::string>> players;
}

namespace engine::types
{

	enum actor_types_t : std::uintptr_t
	{
		player = 0x3F9DCF8
	};

	struct base_t
	{
		std::uintptr_t address;

		template< actor_types_t type >
		bool is_a( )
		{
			const auto vftable = memory::functions::read< std::uintptr_t >( address );

			return vftable == engine::values::process_base + type;
		}

		bool operator==( const base_t& obj )
		{
			return address == obj.address;
		}

		bool operator==( std::uintptr_t comparative )
		{
			return address == comparative;
		}

		operator bool()
		{
			return address != 0;
		}
	};

	template<class t, std::uint64_t offset>
	struct tarray_t
	{
		std::uint32_t get_size(const std::uintptr_t base_address) const
		{
			if (!base_address)
				return 0u;

			return memory::functions::read<std::uint32_t>(base_address + offset + sizeof(std::uintptr_t));
		}

		[[nodiscard]] std::vector<t> get_members(const std::uintptr_t base_address) const
		{
			std::vector<t> members;

			members.reserve(get_size(base_address));

			if (!base_address)
				return members;

			if (const auto list_start = memory::functions::read<std::uintptr_t>(base_address + offset))
			{
				for (auto i = list_start; i < list_start + members.capacity() * sizeof(std::uintptr_t); i += sizeof(std::uintptr_t))
				{
					if (auto member = memory::functions::read<t>(i))
						members.push_back(member);
				}
			}

			return members;
		}
	};

	struct cached_bone_space_t : public base_t
	{
		std::optional<engine::structs::transform_t> get_bone(engine::structs::bones bone) const;
	};

	struct mesh_t : public base_t
	{
		cached_bone_space_t get_cached_bone_space( );

		engine::structs::transform_t get_component_to_world( );
	};

	struct ranged_weapon_t : public base_t
	{
		bool is_aiming();

		float aimed_fov();
	};

	struct player_state_t : public base_t
	{
		engine::structs::fguid_t get_fguid();
	};

	struct pawn_t : public base_t
	{
		mesh_t get_mesh( );

		player_controller_t get_player_controller( );

		player_state_t get_player_state();

		float get_health( );

		float get_max_health( );

		std::uint16_t get_shield();

		std::uint16_t get_max_shield();
		
		ranged_weapon_t get_ranged_weapon( );
	};

	struct actor_t : public base_t
	{
		pawn_t get_pawn( );

		mesh_t get_root_component( );
	};

	struct camera_manager_t : public base_t
	{
		engine::structs::vector_t get_position( );

		engine::structs::vector_t get_rotation( );

		float get_fov( );
	};

	struct character_t : public base_t
	{
		camera_manager_t get_camera_manager( );
	};

	struct player_controller_t : public base_t
	{
		player_state_t get_player_state( );

		bool is_local_player( );

		character_t get_character( );

		camera_manager_t get_camera_manager( );

		pawn_t get_pawn( );
	};

	struct player_t : public base_t
	{
		player_controller_t get_player_controller( );
	};

	struct ugame_instance_t : public base_t
	{
		tarray_t< player_t, offsets::ugameinstance::localplayers > local_players;
	};

	struct ulevel_t : public base_t
	{
		tarray_t<actor_t, engine::offsets::ulevel::actors> actors;

		std::vector<actor_t> get_players( );
	};

	struct gworld_t : public base_t
	{
		ulevel_t get_ulevel( );

		ugame_instance_t get_owning_game_instance( );
	};

	struct fnames_t : public base_t
	{

	};
}

namespace engine::functions
{
	void initiate( );

	engine::structs::matrix4x4_t to_scaled_matrix( const engine::structs::transform_t& transform );

	engine::structs::matrix4x4_t matrix_multiply( const engine::structs::matrix4x4_t& lhs, const engine::structs::matrix4x4_t& rhs );

	std::optional< engine::structs::vector_t > get_bone_position( engine::types::actor_t player, engine::structs::bones bone );

	std::pair< std::int32_t, std::int32_t > get_screen_size( );

	engine::structs::matrix3x4_t get_rotation_matrix( );

	bool world_to_screen( structs::vector_t& in, structs::vector_t& out );

	void populate_players_array( );

	std::string read_fstring(std::uintptr_t object, std::uintptr_t offs);

	bool compare_team(engine::structs::fguid_t actor, engine::structs::fguid_t lp);
}