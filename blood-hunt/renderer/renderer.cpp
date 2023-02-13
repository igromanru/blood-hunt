#include "renderer.hpp"

#include <WinUser.h>
#include <thread>
#include "Oversee/oversee.hpp"
#include "../modules/visuals/esp/esp.hpp"

std::unique_ptr< renderer::structures::renderer_t > renderer::values::renderer;
std::atomic<bool> renderer::values::insert_hit = false;

constexpr const auto str_max_len = 15u;

bool renderer::structures::renderer_t::has_window_moved( const RECT& new_point )
{
	return old_pos.left != new_point.left || old_pos.right != new_point.right || old_pos.top != new_point.top || old_pos.bottom != new_point.bottom;
}

void renderer::structures::renderer_t::release_render_target( )
{
	if( render_target_view )
	{
		render_target_view->Release( );

		render_target_view = nullptr;
	}
}

void renderer::structures::renderer_t::init_imgui( )
{
	ImGui::CreateContext( );

	ImGui_ImplWin32_Init( overlay_window );
	ImGui_ImplDX11_Init( global_device, global_device_context );

	OverseeUI::Render::SetStyles(nullptr);
}

void renderer::structures::renderer_t::create_render_target( )
{
	ID3D11Texture2D* temporary = nullptr;

	global_swap_chain->GetBuffer( 0u, IID_PPV_ARGS( &temporary ) );

	global_device->CreateRenderTargetView( temporary, nullptr, &render_target_view );

	temporary->Release( );
}

void renderer::structures::renderer_t::initialize_device( )
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory( &scd, sizeof( scd ) );

	scd.BufferCount = 2;
	scd.BufferDesc.Width = screen_width;
	scd.BufferDesc.Height = screen_height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 120;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = overlay_window;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL level;
	D3D_FEATURE_LEVEL level_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

	if( D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, level_array, 2, D3D11_SDK_VERSION, &scd, &global_swap_chain, &global_device, &level, &global_device_context ) != S_OK )
	{
		std::printf("D3D PROBLEM\n");

		std::this_thread::sleep_for(std::chrono::seconds(1000));
	}

	create_render_target( );

	ShowWindow( overlay_window, SW_SHOWNORMAL );
	UpdateWindow( overlay_window );
}

void renderer::structures::renderer_t::clear_data( )
{
	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_height = GetSystemMetrics(SM_CYSCREEN);


}

void renderer::structures::renderer_t::render( )
{
	RECT rect{ 0 };
	POINT point{ 0 };

	GetClientRect( game_window, &rect );
	ClientToScreen(game_window, &point );

	rect.left = point.x;
	rect.top = point.y;
	rect.right -= 2;
	rect.bottom -= 2;

	if( has_window_moved( rect ) )
	{
		SetWindowPos( overlay_window, nullptr, rect.left, rect.top, rect.right, rect.bottom, SWP_NOREDRAW );

		old_pos = rect;
	}

	MSG msg{ 0 };
	if ( ::PeekMessageA( &msg, nullptr, 0U, 0U, PM_REMOVE ) )
	{
		::TranslateMessage( &msg );
		::DispatchMessageA( &msg );
	}

	const auto foreground_window = GetForegroundWindow();

	bool overlay_context = (foreground_window == overlay_window || foreground_window == game_window);

	bool alt_tab = (!overlay_context && is_menu_open);

	int ex_style = GetWindowLong(overlay_window, GWL_EXSTYLE);

	if (alt_tab)
	{
		is_menu_open = false;
		SetWindowPos(game_window, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	modules::visuals::esp::callback();

	if (renderer::values::renderer->is_menu_open)
	{
		OverseeUI::Render::RenderMenu();
	}

	ImGui::Render( );

	float clr[4] = { 0, 0, 0 };

	global_device_context->OMSetRenderTargets( 1, &render_target_view, nullptr );
	global_device_context->ClearRenderTargetView( render_target_view, clr);

	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

	global_swap_chain->Present( 0, 0u );

	if (renderer::values::insert_hit)
	{
		renderer::values::insert_hit = false;

		if (overlay_context)
		{
			is_menu_open = !is_menu_open;

			if (is_menu_open)
			{
				SetForegroundWindow(overlay_window);

				SetWindowLong(overlay_window, GWL_EXSTYLE, ex_style & ~WS_EX_TRANSPARENT);
			}
			else
			{
				SetForegroundWindow(game_window);

				SetWindowLong(overlay_window, GWL_EXSTYLE, ex_style | WS_EX_TRANSPARENT);
			}
		}
	}

	if (!is_menu_open)
	{
		SetWindowPos(game_window, overlay_window, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		if (!overlay_context)
			SetWindowPos(overlay_window, game_window, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

renderer::structures::renderer_t::renderer_t( HWND game_window, std::string_view class_name )
	: overlay_window{ CreateWindowExA(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, class_name.data( ), "TeamViewer Panel", WS_POPUP, 1, 1, screen_width, screen_height, 0, 0, 0, 0) },  game_window{ game_window }
{
	SetLayeredWindowAttributes( overlay_window, RGB(0, 0, 0), 240u, LWA_ALPHA );

	MARGINS margins{ -1 };
	DwmExtendFrameIntoClientArea( overlay_window, &margins );

	SetForegroundWindow( overlay_window );

	SetWindowSubclass( overlay_window, &wndproc, 1, 0 );
}

void renderer::start( std::string_view window_name )
{
	const auto game_window = FindWindowA( window_name.data( ), nullptr );

	const auto parent_window = FindWindowA( nullptr, "TeamViewer" );

	const auto name_allocation = std::make_shared< char >( str_max_len );

	GetClassNameA( parent_window, name_allocation.get( ), str_max_len );

	renderer::values::renderer = std::make_unique< renderer::structures::renderer_t >( game_window, name_allocation.get() );

	renderer::values::renderer->initialize_device( );

	renderer::values::renderer->init_imgui( );

	std::thread([=]() {
		RegisterHotKey(0, 1, 0, VK_INSERT);

		MSG hmsg = { 0 };
		while (true)
		{
			if (GetMessage(&hmsg, nullptr, 0, 0) && hmsg.message == WM_HOTKEY && !renderer::values::insert_hit)
				renderer::values::insert_hit = true;
		}
		}).detach();

	while (true) { renderer::values::renderer->render(); };
}
//HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData
LRESULT renderer::wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
	switch (msg)
	{
		case WM_SIZE:
		{
			if( renderer::values::renderer->global_device && wparam != SIZE_MINIMIZED )
			{
				renderer::values::renderer->release_render_target( );

				renderer::values::renderer->screen_width = LOWORD( lparam );
				renderer::values::renderer->screen_height = HIWORD( lparam );

				renderer::values::renderer->global_swap_chain->ResizeBuffers( 0u, renderer::values::renderer->screen_width, renderer::values::renderer->screen_height, DXGI_FORMAT_UNKNOWN, 0u );

				renderer::values::renderer->create_render_target( );

				return S_OK;
			}
			break;
		}
		case WM_SYSCOMMAND:
		{
			if ((wparam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return S_OK;
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}

	if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam ) )
		return S_OK;

	return DefSubclassProc( hwnd, msg, wparam, lparam );
}
