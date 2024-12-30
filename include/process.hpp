#pragma once
#define WIN32_LEAN_AND_MEAN
#include <chrono>
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <optional>
#include <thread>
#include "xor.hpp"

namespace commons::process
{
    inline std::optional<DWORD> get_process_id_by_name(const std::wstring& process_name)
    {
        const HANDLE snapshot{CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};

        if (snapshot == INVALID_HANDLE_VALUE)
        {
            return std::nullopt;
        }

        PROCESSENTRY32 process_entry{};
        process_entry.dwSize = sizeof(PROCESSENTRY32);

        DWORD process_id{0};
        if (Process32First(snapshot, &process_entry))
        {
            do
            {
                if (process_name == process_entry.szExeFile)
                {
                    process_id = process_entry.th32ProcessID;
                    break;
                }
            }
            while (Process32Next(snapshot, &process_entry));
        }

        CloseHandle(snapshot);
        return process_id ? std::optional{process_id} : std::nullopt;
    }

    inline std::optional<HANDLE> open_process_handle(const std::wstring& process_name)
    {
        const auto process_id{get_process_id_by_name(process_name)};
        if (!process_id)
        {
            std::wcout << XORW(L"Process not found: ") << process_name << '\n';
            return std::nullopt;
        }

        HANDLE handle{OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, *process_id)};
        if (!handle)
        {
            std::wcout << XORW(L"Failed to open handle to process. Error: ") << GetLastError() << '\n';
            return std::nullopt;
        }

        return handle;
    }

    inline std::optional<uintptr_t> get_module_base_address(const DWORD& process_id, const std::wstring& module_name,
                                                            const int max_retries = 10,
                                                            const std::chrono::seconds retry_delay = std::chrono::seconds(5))
    {
        std::wcout << XORW(L"Trying to find module: ") << module_name << '\n';

        MODULEENTRY32 module_entry{};
        module_entry.dwSize = sizeof(MODULEENTRY32);

        std::optional<uintptr_t> base_address;
        for (int i{1}; i <= max_retries; ++i)
        {
            const HANDLE snapshot{CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id)};
            if (snapshot == INVALID_HANDLE_VALUE)
            {
                std::wcout << XORW(L"Failed to create module snapshot. Error: ") << GetLastError() << '\n';
                continue;
            }

            if (Module32First(snapshot, &module_entry))
            {
                do
                {
                    if (module_name == module_entry.szModule)
                    {
                        base_address = reinterpret_cast<uintptr_t>(module_entry.modBaseAddr);
                        break;
                    }
                }
                while (Module32Next(snapshot, &module_entry));
            }
            CloseHandle(snapshot);

            if (base_address.has_value())
            {
                break;
            }

            std::wcout << XORW(L"Did not find base address. Attempt: ") << i << '\n';

            if (i < max_retries)
            {
                std::wcout << XORW(L"Will try again after ") << retry_delay << " seconds \n";
                std::this_thread::sleep_for(retry_delay);
            }
        }

        return base_address;
    }
}
