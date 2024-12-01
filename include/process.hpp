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
    inline std::optional<DWORD> GetProcessIdByName(const std::string& processName)
    {
        const HANDLE snapshot{CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};

        if (snapshot == INVALID_HANDLE_VALUE)
        {
            return std::nullopt;
        }

        PROCESSENTRY32 processEntry{};
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        DWORD processId{0};
        if (Process32First(snapshot, &processEntry))
        {
            do
            {
                if (processName == processEntry.szExeFile)
                {
                    processId = processEntry.th32ProcessID;
                    break;
                }
            }
            while (Process32Next(snapshot, &processEntry));
        }

        CloseHandle(snapshot);
        return processId ? std::optional{processId} : std::nullopt;
    }

    inline std::optional<HANDLE> OpenProcessHandle(const std::string& processName)
    {
        const auto processId{GetProcessIdByName(processName)};
        if (!processId)
        {
            std::cout << XOR("Process not found: ") << processName << '\n';
            return std::nullopt;
        }

        HANDLE handle{OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, *processId)};
        if (!handle)
        {
            std::cout << XOR("Failed to open handle to process. Error: ") << GetLastError() << '\n';
            return std::nullopt;
        }

        return handle;
    }

    inline std::optional<uintptr_t> GetModuleBaseAddress(const DWORD& processId, const std::string& moduleName, const int maxRetries = 10, const std::chrono::seconds retryDelay = std::chrono::seconds(5))
    {
        std::cout << XOR("Trying to find module: ") << moduleName << '\n';

        MODULEENTRY32 moduleEntry{};
        moduleEntry.dwSize = sizeof(MODULEENTRY32);

        std::optional<uintptr_t> baseAddress;
        for (int i{1}; i <= maxRetries; ++i)
        {
            const HANDLE snapshot{CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId)};
            if (snapshot == INVALID_HANDLE_VALUE)
            {
                std::cout << XOR("Failed to create module snapshot. Error: ") << GetLastError() << '\n';
                continue;
            }

            if (Module32First(snapshot, &moduleEntry))
            {
                do
                {
                    if (moduleName == moduleEntry.szModule)
                    {
                        baseAddress = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                        break;
                    }
                }
                while (Module32Next(snapshot, &moduleEntry));
            }
            CloseHandle(snapshot);

            if (baseAddress.has_value())
            {
                break;
            }

            std::cout << XOR("Did not find base address. Attempt: ") << i << '\n';

            if (i < maxRetries)
            {
                std::cout << XOR("Will try again after ") << retryDelay << " seconds \n";
                std::this_thread::sleep_for(retryDelay);
            }
        }

        return baseAddress;
    }
}
