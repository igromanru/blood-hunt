#include "memory.hpp"

#include <TlHelp32.h>
#include "driver/driver.hpp"

memory::types::managed_t< HANDLE, CloseHandle > memory::values::process_handle;

HWND memory::values::process_window;

DWORD memory::values::process_id;

std::uintptr_t memory::values::process_base;

std::string memory::functions::read_string( std::uintptr_t address, const bool inner )
{
    if ( inner )
        address = read< std::uintptr_t >( address );

    std::string buffer;

    do
        buffer.push_back( read< char >( address++ ) );
    while ( read< char >( address ) );

    return buffer;
}

std::uintptr_t memory::functions::get_module( const char* const module_name )
{
    if ( const auto process_snapshot = types::managed_t< HANDLE, CloseHandle >{ CreateToolhelp32Snapshot( TH32CS_SNAPMODULE,  memory::values::process_id) }; process_snapshot.get( ) != INVALID_HANDLE_VALUE )
    {
        auto module_entry = MODULEENTRY32{ sizeof( MODULEENTRY32 ) };

        do
        {
            if ( !std::strcmp( module_entry.szModule, module_name ) )
                return reinterpret_cast< std::uintptr_t >( module_entry.modBaseAddr );
        } while ( Module32Next( process_snapshot.get( ), &module_entry ) );
    }

    return 0;
}

void memory::functions::initiate( const char* const process_window_name )
{
    values::process_window = FindWindowA( process_window_name, nullptr );

    GetWindowThreadProcessId(values::process_window, &values::process_id);

    values::process_handle = types::managed_t< HANDLE, CloseHandle >{ LI_FN( OpenProcess )( PROCESS_ALL_ACCESS, false,  memory::values::process_id) };

    driver::initialize(process_window_name);

    values::process_base = driver::get_process_base();
}