#pragma once
#include <Windows.h>
#include <iostream>
#include "../../dependencies/xorstring/xorstring.hpp"

struct communication_flags
{
	int id;
	int pid;
	UINT64 param0;
	UINT64 param1;
	UINT64 param2;
	DWORD reason;
	PVOID ptr;
	PVOID ret;
};

using ngppd = std::uint64_t(*)(PVOID, PVOID, PVOID, int, int);
inline ngppd NtGdiPolyPolyDraw = nullptr;

namespace driver
{
	uintptr_t get_process_base();

	inline void setup(int pid)
	{
		LoadLibraryA(xorstr_("user32.dll"));
		auto win32u = LoadLibraryA(xorstr_("win32u.dll"));
		if (*reinterpret_cast<BYTE*>(GetProcAddress) == 0xE9 || !win32u)
		{
			std::cout << xorstr_("lmao really") << std::endl;
			std::cin.get();

			exit(0);
		}

		auto addr = GetProcAddress(win32u, xorstr_("NtGdiPolyPolyDraw"));

		*reinterpret_cast<PVOID*>(&NtGdiPolyPolyDraw) = addr;

		communication_flags request;

		request.id = 0;
		request.reason = 0x1000;
		request.pid = pid;

		NtGdiPolyPolyDraw(nullptr, &request, nullptr, 1, 2);
	}

	inline bool is_driver_loaded()
	{
		communication_flags request;

		request.id = 10;
		request.reason = 0x1000;
		request.ret = nullptr;

		NtGdiPolyPolyDraw(nullptr, &request, nullptr, 1, 2);
		return request.ret;
	}

	inline uintptr_t get_process_base()
	{
		communication_flags request;

		request.id = 1;
		request.reason = 0x1000;
		request.ret = nullptr;

		NtGdiPolyPolyDraw(nullptr, &request, nullptr, 1, 2);
		return reinterpret_cast<uintptr_t>(request.ret);
	}

	template <typename type>
	inline type read_virtual_memory(UINT64 addr)
	{
		type response{};
		communication_flags request;

		request.id = 2;
		request.reason = 0x1000;
		request.param0 = addr;
		request.param1 = sizeof(type);
		request.ret = &response;

		NtGdiPolyPolyDraw(nullptr, &request, nullptr, 1, 2);
		return response;
	}

	template <typename type>
	inline void write_virtual_memory(UINT64 addr, type value)
	{
		type input = value;
		communication_flags request;

		request.id = 3;
		request.reason = 0x1000;
		request.param0 = addr;
		request.ptr = &input;
		request.param1 = sizeof(value);

		NtGdiPolyPolyDraw(nullptr, &request, nullptr, 1, 2);
	}

	inline std::string read_string(UINT64 addr)
	{
		auto buffer = addr;

		std::string string_buffer;
		do
			string_buffer.push_back(read_virtual_memory<char>(buffer++));
		while (read_virtual_memory<char>(buffer) != '\0');

		return string_buffer;
	}

	inline void initialize(std::string game_name)
	{
		const auto game_window = FindWindowA(game_name.c_str(), nullptr);

		DWORD pid;
		auto test = GetWindowThreadProcessId(game_window, &pid);

		if (!test || !pid)
		{
			std::cout << xorstr_("game not found!") << std::endl;
			std::cin.get();
			throw std::exception("game not found");
		}

		driver::setup(pid);

		if (!driver::is_driver_loaded())
		{
			std::cout << xorstr_("driver not loaded!") << std::endl;
			std::cin.get();
			throw std::exception("driver no loaded");
		}

	}
}