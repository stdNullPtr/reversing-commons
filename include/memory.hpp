#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <optional>
#include <Windows.h>
#include "xor.hpp"

namespace commons::mem
{
    template <typename T>
    std::optional<T> read(const HANDLE& process_handle, const uintptr_t& address)
    {
        T buffer;
        SIZE_T bytes_read;

        if (ReadProcessMemory(process_handle, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytes_read) && bytes_read == sizeof(T))
        {
            return buffer;
        }

        std::wcout << XORW(L"Failed to read memory at address ") << std::hex << std::uppercase << address << XORW(L". Error: ") << GetLastError() << '\n';
        return std::nullopt;
    }

    template <typename T>
    bool write(const HANDLE& process_handle, const uintptr_t& address, const T& data)
    {
        SIZE_T bytes_written;
        if (WriteProcessMemory(process_handle, reinterpret_cast<LPVOID>(address), &data, sizeof(T), &bytes_written) && bytes_written == sizeof(T))
        {
            return true;
        }

        std::wcout << XORW(L"Failed to write memory at address") << std::hex << std::uppercase << address << XORW(L".Error: ") << GetLastError() << '\n';
        return false;
    }
}
