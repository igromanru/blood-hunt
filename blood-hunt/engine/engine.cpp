#include "engine.hpp"
#include <ranges>
#include <cmath>

using namespace engine::types;

std::atomic < gworld_t > engine::values::gworld;
fnames_t engine::values::fnames;
HWND engine::values::game_handle;
std::atomic < std::uintptr_t > engine::values::process_base;

std::mutex engine::values::lock;
std::vector<std::pair<engine::types::actor_t, std::string>> engine::values::players;

constexpr auto bone_size = 0x30u;
constexpr auto M_PI = 3.14159265358979323846f;

std::optional< engine::structs::transform_t > engine::types::cached_bone_space_t::get_bone( engine::structs::bones bone ) const
{
	if (!address)
		return std::nullopt;

	if ( !memory::functions::read< std::uintptr_t >( address + static_cast< std::uint64_t >( bone ) * bone_size ) )
		return std::nullopt;

	return memory::functions::read< engine::structs::transform_t >( address + static_cast< std::uint64_t >( bone ) * bone_size );
}

engine::types::cached_bone_space_t engine::types::mesh_t::get_cached_bone_space( )
{
	return memory::functions::read< engine::types::cached_bone_space_t >( address + engine::offsets::mesh::cached_bone_space );
}

engine::structs::transform_t engine::types::mesh_t::get_component_to_world()
{
	return memory::functions::read< engine::structs::transform_t >( address + engine::offsets::mesh::component_to_world );
}

mesh_t engine::types::pawn_t::get_mesh( )
{
	return memory::functions::read< mesh_t >( address + engine::offsets::pawn::mesh );
}

player_controller_t engine::types::pawn_t::get_player_controller()
{
	return memory::functions::read< player_controller_t >( address + engine::offsets::pawn::player_controller );
}

player_state_t engine::types::pawn_t::get_player_state()
{
	return memory::functions::read< player_state_t >(address + engine::offsets::pawn::player_state);
}

float engine::types::pawn_t::get_health()
{
	return memory::functions::read< float >( address + engine::offsets::pawn::health );
}

float engine::types::pawn_t::get_max_health()
{
	return memory::functions::read< float >( address + engine::offsets::pawn::max_health );
}

std::uint16_t engine::types::pawn_t::get_shield()
{
	return memory::functions::read< uint16_t >(address + engine::offsets::pawn::shield);
}

std::uint16_t engine::types::pawn_t::get_max_shield()
{
	return memory::functions::read< uint16_t >(address + engine::offsets::pawn::max_shield);
}

bool engine::types::ranged_weapon_t::is_aiming()
{
	return memory::functions::read< bool >(address + engine::offsets::ranged_weapon::is_aiming);
}

float engine::types::ranged_weapon_t::aimed_fov()
{
	return memory::functions::read< float >(address + engine::offsets::ranged_weapon::aimed_fov);
}

ranged_weapon_t engine::types::pawn_t::get_ranged_weapon()
{
	return memory::functions::read< ranged_weapon_t >( address + engine::offsets::pawn::ranged_weapon);
}

pawn_t engine::types::actor_t::get_pawn(  )
{
	return memory::functions::read< pawn_t >( address + engine::offsets::actor::pawn );
}

mesh_t engine::types::actor_t::get_root_component()
{
	return memory::functions::read< mesh_t >( address + engine::offsets::actor::root_component );
}

engine::structs::vector_t engine::types::camera_manager_t::get_position( )
{
	return memory::functions::read< structs::vector_t >( address + engine::offsets::camera_manager::camera_cache_private + engine::offsets::camera_manager::pov + engine::offsets::camera_manager::location );
}

engine::structs::vector_t engine::types::camera_manager_t::get_rotation( )
{
	return memory::functions::read< structs::vector_t >( address + engine::offsets::camera_manager::camera_cache_private + engine::offsets::camera_manager::pov + engine::offsets::camera_manager::rotation );
}

float engine::types::camera_manager_t::get_fov( )
{
	return memory::functions::read< float >( address + engine::offsets::camera_manager::camera_cache_private + engine::offsets::camera_manager::pov + engine::offsets::camera_manager::fov );
}

camera_manager_t engine::types::character_t::get_camera_manager( )
{
	return memory::functions::read< camera_manager_t >( address + engine::offsets::character::camera_manager );
}

player_state_t engine::types::player_controller_t::get_player_state()
{
	return memory::functions::read< player_state_t >(address + engine::offsets::player_controller::player_state);
}

bool engine::types::player_controller_t::is_local_player( )
{
	return memory::functions::read< bool >(address + engine::offsets::player_controller::is_local_player );
}

character_t engine::types::player_controller_t::get_character()
{
	return memory::functions::read< character_t >( address + engine::offsets::player_controller::character );
}

camera_manager_t engine::types::player_controller_t::get_camera_manager()
{
	return memory::functions::read< camera_manager_t >(address + engine::offsets::character::camera_manager );
}

pawn_t engine::types::player_controller_t::get_pawn( )
{
	return memory::functions::read< pawn_t >( address + engine::offsets::player_controller::pawn );
}

player_controller_t engine::types::player_t::get_player_controller( )
{
	return memory::functions::read< player_controller_t >( address + engine::offsets::player::player_controller );
}

std::vector< actor_t > engine::types::ulevel_t::get_players( )
{
	std::vector< actor_t > players;

	for ( const auto actors_list = actors.get_members( address ); auto actor : actors_list )
	{
		//if (!actor.address)
		//	continue;

		//auto pawn = actor.get_pawn();

		//if (!pawn.address)
		//	continue;

		//if (pawn.get_player_controller().address)
		//	continue;

		//auto player_state = memory::functions::read< std::uintptr_t >(pawn.address + engine::offsets::pawn::player_state);

		//if (!player_state)
		//	continue;

		//std::printf("%p %s\n", driver::read_virtual_memory<std::uintptr_t>(actor.address) - memory::values::process_base, engine::functions::read_fstring(player_state, engine::offsets::player_state::player_name_private).c_str());

		if ( actor.is_a< actor_types_t::player >( ) )
			players.push_back( actor );
	}

	return players;
}

ugame_instance_t engine::types::gworld_t::get_owning_game_instance( )
{
	return memory::functions::read< ugame_instance_t >( address + engine::offsets::gworld::owning_game_instance );
}

ulevel_t engine::types::gworld_t::get_ulevel( )
{
	return memory::functions::read< ulevel_t >( address + engine::offsets::gworld::ulevel );
}

engine::structs::fguid_t engine::types::player_state_t::get_fguid()
{
	return memory::functions::read< engine::structs::fguid_t >(address + engine::offsets::player_state::group_id);
}

void engine::functions::initiate( )
{
	engine::values::game_handle = FindWindowA( "UnrealEngine", nullptr );

	engine::values::process_base = memory::values::process_base;

	engine::values::gworld = memory::functions::read< gworld_t >( engine::values::process_base + engine::offsets::core::gworld );
	engine::values::fnames = fnames_t{ engine::values::process_base + engine::offsets::core::fnamearray };
}

engine::structs::matrix4x4_t engine::functions::to_scaled_matrix( const engine::structs::transform_t& transform )
{
	engine::structs::matrix4x4_t matrix;

	matrix.matrix4x4[0][3] = transform.translation.x;
	matrix.matrix4x4[1][3] = transform.translation.y;
	matrix.matrix4x4[2][3] = transform.translation.z;

	const auto x2 = transform.rotation.x + transform.rotation.x;
	const auto y2 = transform.rotation.y + transform.rotation.y;
	const auto z2 = transform.rotation.z + transform.rotation.z;

	const auto xx2 = transform.rotation.x * x2;
	const auto yy2 = transform.rotation.y * y2;
	const auto zz2 = transform.rotation.z * z2;

	matrix.matrix4x4[ 0 ][ 0 ] = ( 1.0f - ( yy2 + zz2 ) ) * transform.scale_3d.x;
	matrix.matrix4x4[ 1 ][ 1 ] = ( 1.0f - ( xx2 + zz2 ) ) * transform.scale_3d.y;
	matrix.matrix4x4[ 2 ][ 2 ] = ( 1.0f - ( xx2 + yy2 ) ) * transform.scale_3d.z;

	const auto yz2 = transform.rotation.y * z2;
	const auto wx2 = transform.rotation.w * x2;

	matrix.matrix4x4[ 1 ][ 2 ] = ( yz2 - wx2 ) * transform.scale_3d.z;
	matrix.matrix4x4[ 2 ][ 1 ] = ( yz2 + wx2 ) * transform.scale_3d.y;

	const auto xy2 = transform.rotation.x * y2;
	const auto wz2 = transform.rotation.w * z2;

	matrix.matrix4x4[ 0 ][ 1 ] = ( xy2 - wz2 ) * transform.scale_3d.y;
	matrix.matrix4x4[ 1 ][ 0 ] = ( xy2 + wz2 ) * transform.scale_3d.x;

	const auto xz2 = transform.rotation.x * z2;
	const auto wy2 = transform.rotation.w * y2;

	matrix.matrix4x4[ 0 ][ 2 ] = ( xz2 + wy2 ) * transform.scale_3d.z;
	matrix.matrix4x4[ 2 ][ 0 ] = ( xz2 - wy2 ) * transform.scale_3d.x;

	matrix.matrix4x4[ 3 ][ 0 ] = 0.0f;
	matrix.matrix4x4[ 3 ][ 1 ] = 0.0f;
	matrix.matrix4x4[ 3 ][ 2 ] = 0.0f;
	matrix.matrix4x4[ 3 ][ 3 ] = 1.0f;

	return matrix;
}

engine::structs::matrix4x4_t engine::functions::matrix_multiply( const engine::structs::matrix4x4_t& lhs, const engine::structs::matrix4x4_t& rhs )
{
	engine::structs::matrix4x4_t matrix;

	const auto a00 = lhs.matrix4x4[ 0 ][ 0 ]; const auto a01 = lhs.matrix4x4[ 0 ][ 1 ]; const auto a02 = lhs.matrix4x4[ 0 ][ 2 ]; const auto a03 = lhs.matrix4x4[ 0 ][ 3 ];
	const auto a10 = lhs.matrix4x4[ 1 ][ 0 ]; const auto a11 = lhs.matrix4x4[ 1 ][ 1 ]; const auto a12 = lhs.matrix4x4[ 1 ][ 2 ]; const auto a13 = lhs.matrix4x4[ 1 ][ 3 ];
	const auto a20 = lhs.matrix4x4[ 2 ][ 0 ]; const auto a21 = lhs.matrix4x4[ 2 ][ 1 ]; const auto a22 = lhs.matrix4x4[ 2 ][ 2 ]; const auto a23 = lhs.matrix4x4[ 2 ][ 3 ];
	const auto a30 = lhs.matrix4x4[ 3 ][ 0 ]; const auto a31 = lhs.matrix4x4[ 3 ][ 1 ]; const auto a32 = lhs.matrix4x4[ 3 ][ 2 ]; const auto a33 = lhs.matrix4x4[ 3 ][ 3 ];

	const auto b00 = rhs.matrix4x4[ 0 ][ 0 ]; const auto b01 = rhs.matrix4x4[ 0 ][ 1 ]; const auto b02 = rhs.matrix4x4[ 0 ][ 2 ]; const auto b03 = rhs.matrix4x4[ 0 ][ 3 ];
	const auto b10 = rhs.matrix4x4[ 1 ][ 0 ]; const auto b11 = rhs.matrix4x4[ 1 ][ 1 ]; const auto b12 = rhs.matrix4x4[ 1 ][ 2 ]; const auto b13 = rhs.matrix4x4[ 1 ][ 3 ];
	const auto b20 = rhs.matrix4x4[ 2 ][ 0 ]; const auto b21 = rhs.matrix4x4[ 2 ][ 1 ]; const auto b22 = rhs.matrix4x4[ 2 ][ 2 ]; const auto b23 = rhs.matrix4x4[ 2 ][ 3 ];
	const auto b30 = rhs.matrix4x4[ 3 ][ 0 ]; const auto b31 = rhs.matrix4x4[ 3 ][ 1 ]; const auto b32 = rhs.matrix4x4[ 3 ][ 2 ]; const auto b33 = rhs.matrix4x4[ 3 ][ 3 ];

	matrix.matrix4x4[ 0 ][ 0 ] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
	matrix.matrix4x4[ 0 ][ 1 ] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
	matrix.matrix4x4[ 0 ][ 2 ] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
	matrix.matrix4x4[ 0 ][ 3 ] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
 
	matrix.matrix4x4[ 1 ][ 0 ] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
	matrix.matrix4x4[ 1 ][ 1 ] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
	matrix.matrix4x4[ 1 ][ 2 ] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
	matrix.matrix4x4[ 1 ][ 3 ] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;

	matrix.matrix4x4[ 2 ][ 0 ] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
	matrix.matrix4x4[ 2 ][ 1 ] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
	matrix.matrix4x4[ 2 ][ 2 ] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
	matrix.matrix4x4[ 2 ][ 3 ] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;

	matrix.matrix4x4[ 3 ][ 0 ] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
	matrix.matrix4x4[ 3 ][ 1 ] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
	matrix.matrix4x4[ 3 ][ 2 ] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
	matrix.matrix4x4[ 3 ][ 3 ] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;

	return matrix;
}

std::optional< engine::structs::vector_t > engine::functions::get_bone_position( engine::types::actor_t player, engine::structs::bones bone )
{
	if (!player.address)
		return std::nullopt;

	auto pawn = player.get_pawn();

	if (!pawn.address)
		return std::nullopt;

	auto mesh = pawn.get_mesh();

	if (!mesh.address)
		return std::nullopt;

	const auto bone_array = memory::functions::read<uintptr_t>(mesh.address + 0x4B0 + memory::functions::read<int>(mesh.address + 0x4F4) * 16);

	if (!bone_array)
		return std::nullopt;

	const auto bone_transform = memory::functions::read<engine::structs::transform_t>(bone_array + static_cast<std::uint64_t>(bone) * 0x30);

	//auto cbs = mesh.get_cached_bone_space();

	//if (!cbs.address)
	//	return std::nullopt;

	//const auto bone_transform = cbs.get_bone( bone );

	//if ( !bone_transform.has_value( ) )
	//	return std::nullopt;

	const auto matrix = matrix_multiply( to_scaled_matrix( bone_transform ) , to_scaled_matrix( mesh.get_component_to_world( ) ) );

	return std::make_optional< engine::structs::vector_t >( matrix.matrix4x4[ 0 ][ 3 ], matrix.matrix4x4[ 1 ][ 3 ], matrix.matrix4x4[ 2 ][ 3 ] );
}

static HWND game;

std::pair< std::int32_t, std::int32_t > engine::functions::get_screen_size( )
{
	if (!game)
		game = FindWindowA("UnrealWindow", 0);

	RECT screen_dimensions{ };

	if( GetClientRect(game, &screen_dimensions ) )
	{
		return { screen_dimensions.right - screen_dimensions.left, screen_dimensions.bottom - screen_dimensions.top };
	}

	return { 0, 0 };
}

engine::structs::matrix3x4_t engine::functions::get_rotation_matrix( )
{
	structs::matrix3x4_t rotation_matrix{};

	auto thread_safe_gworld = memory::functions::read< gworld_t >(engine::values::process_base + engine::offsets::core::gworld);

	auto owning_game_instance = thread_safe_gworld.get_owning_game_instance();

	auto camera_rotation = owning_game_instance.local_players.get_members( owning_game_instance.address ).front( ).get_player_controller( ).get_camera_manager( ).get_rotation( );

	const auto p = camera_rotation.x * M_PI / 180;
	const auto y = camera_rotation.y * M_PI / 180;
	const auto r = camera_rotation.z * M_PI / 180;

	const auto sp = std::sinf( p ), cp = std::cosf( p ), sy = std::sinf( y ), cy = std::cosf( y ), sr = std::sinf( r ), cr = std::cosf( r );

	rotation_matrix.matrix3x4[ 0 ][ 0 ] = cp * cy; rotation_matrix.matrix3x4[ 0 ][ 1 ] = cp * sy; rotation_matrix.matrix3x4[ 0 ][ 2 ] = sp; rotation_matrix.matrix3x4[ 0 ][ 3 ] = 0.f;
	rotation_matrix.matrix3x4[ 1 ][ 0 ] = sr * sp * cy - cr * sy; rotation_matrix.matrix3x4[ 1 ][ 1 ] = sr * sp * sy + cr * cy; rotation_matrix.matrix3x4[ 1 ][ 2 ] = -sr * cp; rotation_matrix.matrix3x4[ 1 ][ 3 ] = 0.f;
	rotation_matrix.matrix3x4[ 2 ][ 0 ] = -(cr * sp * cy + sr * sy); rotation_matrix.matrix3x4[ 2 ][ 1 ] = cy * sr - cr * sp * sy; rotation_matrix.matrix3x4[ 2 ] [2 ] = cr * cp; rotation_matrix.matrix3x4[ 2 ][ 3 ] = 0.f;

	return rotation_matrix;
}

bool engine::functions::world_to_screen( structs::vector_t& in, structs::vector_t& out )
{
	const auto rotation_matrix = get_rotation_matrix( );

	auto thread_safe_gworld = memory::functions::read< gworld_t >(engine::values::process_base + engine::offsets::core::gworld);

	if (!thread_safe_gworld.address)
		return false;

	auto owning_game_instance = thread_safe_gworld.get_owning_game_instance( );

	if (!owning_game_instance)
		return false;

	auto lp = owning_game_instance.local_players.get_members(owning_game_instance.address).front();

	if (!lp)
		return false;

	auto pc = lp.get_player_controller();

	if (!pc)
		return false;

	auto camera_manager = pc.get_camera_manager( );

	if (!camera_manager)
		return false;

	const auto position = camera_manager.get_position( );

	auto camera_fov = camera_manager.get_fov( ) + 0x18;

	if (lp.address)
	{

		if (pc.address)
		{


			auto pawn = pc.get_pawn();

			//std::printf("%p\n", pawn);

			if (pawn.address)
			{

				auto rw = pawn.get_ranged_weapon();

				if (rw.address && rw.is_aiming())
				{
					auto aimed_fov = rw.aimed_fov();

					//std::printf("%f\n", aimed_fov);

					if (aimed_fov)
						camera_fov -= 0x18;
				}
			}
		}
	}

	structs::vector_t x{ rotation_matrix.matrix3x4[ 0 ][ 0 ], rotation_matrix.matrix3x4[ 0 ][ 1 ], rotation_matrix.matrix3x4[ 0 ][ 2 ] };
	structs::vector_t y{ rotation_matrix.matrix3x4[ 1 ][ 0 ], rotation_matrix.matrix3x4[ 1 ][ 1 ], rotation_matrix.matrix3x4[ 1 ][ 2 ] };
	structs::vector_t z{ rotation_matrix.matrix3x4[ 2 ][ 0 ], rotation_matrix.matrix3x4[ 2 ][ 1 ], rotation_matrix.matrix3x4[ 2 ][ 2 ] };

	const auto delta = in - position;

	structs::vector_t transformed{ delta.dot( y ), delta.dot( z ), delta.dot( x ) };

	transformed.z = transformed.z < 1.f ? 1.f : transformed.z;

	const auto [ width, height ] = get_screen_size( );

	const auto center_x = width / 2.0f;
	const auto center_y = height / 2.0f;

	auto fov = std::tanf( camera_fov * M_PI / 360.f );

	out.x = center_x + transformed.x * ( center_x / fov ) / transformed.z;
	out.y = center_y - transformed.y * ( center_x / fov ) / transformed.z;

	if ( out.x > width || out.x < 0 || out.y > height || out.y < 0 )
		return false;

	return true;
}

std::vector<std::pair<engine::types::actor_t, std::string>> temp_players;

void engine::functions::populate_players_array( )
{
	initiate();

	temp_players.clear( );

	auto thread_safe_gworld = memory::functions::read< gworld_t >(engine::values::process_base + engine::offsets::core::gworld);

	if (!thread_safe_gworld.address)
		return;

	auto ulevel = thread_safe_gworld.get_ulevel();

	if (!ulevel.address)
		return;

	auto owning_game_instance = thread_safe_gworld.get_owning_game_instance();

	if (!owning_game_instance.address)
		return;

	for (const auto players = ulevel.get_players(); auto player : players) 
	{
		if (!player.address)
			continue;

		auto pawn = player.get_pawn();

		if (!pawn.address)
			continue;

		if (pawn.get_player_controller().address)
			continue;

		auto player_state = memory::functions::read< std::uintptr_t >(pawn.address + engine::offsets::pawn::player_state);

		if (!player_state)
			continue;

		temp_players.push_back({ { player }, read_fstring(player_state, engine::offsets::player_state::player_name_private) });
	}

	std::unique_lock locker(engine::values::lock);

	engine::values::players = temp_players;
}

std::string engine::functions::read_fstring(std::uintptr_t object, std::uintptr_t offs)
{
	auto buffer = memory::functions::read< std::uintptr_t >(object + offs);

	if (!buffer)
		return {};

	std::wstring string_buffer;
	do
	{
		if (memory::functions::read<char>(buffer) == '\0')
		{
			++buffer;

			continue;
		}

		string_buffer.push_back(memory::functions::read<char>(buffer++));
	} while (memory::functions::read<char>(buffer) != '\0' || memory::functions::read<char>(buffer + 1) != '\0');

	return std::string(string_buffer.begin(), string_buffer.end());
}

bool engine::functions::compare_team(engine::structs::fguid_t actor, engine::structs::fguid_t lp)
{
	if ((!lp.a && !lp.b && !lp.c && !lp.d) || (!actor.a && !actor.b && !actor.c && !actor.d))
		return true;

	return (actor.a ^ lp.a | actor.b ^ lp.b | actor.c ^ lp.c | actor.d ^ lp.d);
}
