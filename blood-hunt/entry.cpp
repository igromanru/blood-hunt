#include <Windows.h>
#include <cstdint>
#include <thread>

#include "renderer/renderer.hpp"
#include "memory/memory.hpp"
#include "settings/settings.hpp"
#include "engine/engine.hpp"
#include "modules/player/aimbot/aimbot.hpp"
#include "modules/player/antiaim/antiaim.hpp"
#include "modules/player/modifiers/modifiers.hpp"

__forceinline void open_console( const char* const console_name )
{
	FILE* input_output_stream;

	AllocConsole( );

	freopen_s( &input_output_stream, "CONIN$", "r", stdin );
	freopen_s( &input_output_stream, "CONOUT$", "w", stdout );
	freopen_s( &input_output_stream, "CONOUT$", "w", stderr );

	SetConsoleTitleA( console_name );
}

int main( )
{
	memory::functions::initiate("UnrealWindow");

	engine::functions::initiate();

	std::thread([]()
		{
			while (true)
			{
				engine::functions::populate_players_array();
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}).detach();

	std::thread([]()
		{
			while (true)
			{
				modules::players::aimbot::callback();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}).detach();

	std::thread([]()
		{
			while (true)
			{
				modules::players::antiaim::callback();

				modules::players::modifiers::callback();

				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}).detach();

	std::thread{ renderer::start, "UnrealWindow" }.detach();

	if constexpr (settings::values::debug)
		open_console("blood-hunt-client");

	while (true);
	

	return 0;
}

bool __stdcall DllMain( void*, const std::uint32_t injection_reason, void* )
{
	if ( injection_reason == DLL_PROCESS_ATTACH )
		std::thread{ main }.detach();

	return true;
}