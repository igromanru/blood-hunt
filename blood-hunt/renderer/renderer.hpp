#pragma once

#include <Windows.h>
#include <memory>
#include <string_view>

#include <dwmapi.h>
#pragma comment (lib, "dwmapi")

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_dx11.h"
#include "../dependencies/imgui/imgui_impl_win32.h"
#include "../dependencies/imgui/imgui_internal.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")

#pragma comment(lib, "Comctl32.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace renderer::structures
{
	struct renderer_t
	{
		IDXGISwapChain* global_swap_chain = nullptr;
		ID3D11Device* global_device = nullptr;
		ID3D11DeviceContext* global_device_context = nullptr;
		ID3D11RenderTargetView* render_target_view = nullptr;

		std::uint32_t screen_width = GetSystemMetrics( SM_CXSCREEN );
		std::uint32_t screen_height = GetSystemMetrics( SM_CYSCREEN );

		HWND overlay_window, game_window;

		RECT old_pos{ 0 };

		bool is_menu_open = false;

		bool has_window_moved( const RECT& new_point );

		void release_render_target( );

		void init_imgui( );

		void create_render_target( );

		void initialize_device( );

		void clear_data( );

		void render( );

		renderer_t( HWND game_window, std::string_view class_name );
	};
}

namespace renderer::values
{
	extern std::unique_ptr< renderer::structures::renderer_t > renderer;

	extern std::atomic<bool> insert_hit;
}

namespace renderer
{
	void start( std::string_view window_name );

	LRESULT wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData );
}