#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <optional>
#include <print>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <keyboard.hpp>
#include <xor.hpp>
#include <console.hpp>

namespace commons::window
{
	using namespace std::chrono_literals;

	inline std::optional<std::string> getFocusedWindowTitle()
	{
		const HWND fgWindowHandle{GetForegroundWindow()};
		if (fgWindowHandle == nullptr)
		{
			std::println(XOR("Can't get focused window handle, or there is no focused window."));
			return std::nullopt;
		}

		const int titleLength{GetWindowTextLength(fgWindowHandle)};
		if (titleLength == 0)
		{
			std::println(XOR("Window title is 0 characters."));
			return std::nullopt;
		}

		std::vector<char> windowTitle(titleLength + 1);
		GetWindowText(fgWindowHandle, windowTitle.data(), titleLength + 1);

		return std::string(windowTitle.data());
	}

	inline bool isWindowInFocus(const std::string& windowName)
	{
		const auto focusedWindowTitle{getFocusedWindowTitle()};

		if (!focusedWindowTitle.has_value())
		{
			std::println(XOR("Focused window title is empty."));
			return false;
		}
		return focusedWindowTitle.value() == windowName;
	}

	inline bool isWindowRunning(const std::string& windowName)
	{
		return FindWindow(nullptr, windowName.c_str()) != nullptr;
	}

	inline bool waitForWindow(const std::string& windowName, const std::chrono::milliseconds& timeout)
	{
		const auto start{std::chrono::steady_clock::now()};

		while (std::chrono::steady_clock::now() - start < timeout)
		{
			console::clearConsole({0, 0});
			std::println(XOR("Waiting for window '{}' (INS to abort)..."), windowName);

			if (isWindowRunning(windowName))
			{
				std::println(XOR("Window '{}' was found."), windowName);
				return true;
			}

			if (keyboard::isKeyPressed(VK_INSERT))
			{
				std::println(XOR("Exit key pressed. Aborting waiting for window '{}'."), windowName);
				return false;
			}

			std::this_thread::sleep_for(100ms);
		}

		std::println(XOR("Timeout reached; window '{}' was not found."), windowName);
		return false;
	}
}
