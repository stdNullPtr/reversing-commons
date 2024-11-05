#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <optional>
#include <print>
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
			std::println(XOR("Process not found: {}"), processName);
			return std::nullopt;
		}

		HANDLE handle{OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, *processId)};
		if (!handle)
		{
			std::println(XOR("Failed to open handle to process. Error: {}"), GetLastError());
			return std::nullopt;
		}

		return handle;
	}

	inline std::optional<uintptr_t> GetModuleBaseAddress(const DWORD& processId, const std::string& moduleName)
	{
		const HANDLE snapshot{CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId)};
		if (snapshot == INVALID_HANDLE_VALUE)
		{
			std::println(XOR("Failed to create module snapshot. Error: {}"), GetLastError());
			return std::nullopt;
		}

		MODULEENTRY32 moduleEntry{};
		moduleEntry.dwSize = sizeof(MODULEENTRY32);

		std::optional<uintptr_t> baseAddress;
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
		return baseAddress;
	}
}
