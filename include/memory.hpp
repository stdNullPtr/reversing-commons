#pragma once
#define WIN32_LEAN_AND_MEAN
#include <optional>
#include <Windows.h>
#include <print>
#include "xor.hpp"

namespace commons::mem
{
	template <typename T>
	std::optional<T> read(const HANDLE& processHandle, const uintptr_t& address)
	{
		T buffer;
		SIZE_T bytesRead;

		if (ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytesRead)
			&& bytesRead == sizeof(T))
		{
			return buffer;
		}

		std::println(XOR("Failed to read memory at address 0x{:X}. Error: 0x{:X}"), address, GetLastError());
		return std::nullopt;
	}

	template <typename T>
	bool write(const HANDLE& processHandle, const uintptr_t& address, const T& data)
	{
		SIZE_T bytesWritten;
		if (WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), &data, sizeof(T), &bytesWritten)
			&& bytesWritten == sizeof(T))
		{
			return true;
		}

		std::println(XOR("Failed to write memory at address 0x{:X}. Error: 0x{:X}"), address, GetLastError());
		return false;
	}
}
