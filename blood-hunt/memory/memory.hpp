#pragma once

#include <Windows.h>
#include <cstdint>
#include <string>
#include <memory>
#include "../dependencies/lazy_importer/lazy_importer.hpp"

#include "driver/driver.hpp"

namespace memory::types
{
	template < class type_t, auto deleter_t >
	using managed_t = std::unique_ptr< std::remove_pointer_t< type_t >, std::integral_constant< decltype( deleter_t ), deleter_t > >;
}

namespace memory::values
{
	extern types::managed_t< HANDLE, CloseHandle > process_handle;

	extern HWND process_window;

	extern DWORD process_id;

	extern std::uintptr_t process_base;
}

namespace memory::functions
{
	template < typename value_t >
	__declspec(noinline) value_t read(const std::uintptr_t address)
	{
		return driver::read_virtual_memory<value_t>(address);
	}

	template < typename value_t >
	__declspec(noinline) void write(const std::uintptr_t address, const value_t& value)
	{
		driver::write_virtual_memory<value_t>(address, value);
	}

	void initiate(const char* const process_window_name);

	std::string read_string( std::uintptr_t address, const bool inner = true );

	std::uintptr_t get_module( const char* const module_name );

}