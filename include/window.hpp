#pragma once
#define WIN32_LEAN_AND_MEAN
#include <optional>
#include <print>
#include <string>
#include <vector>

#include "Windows.h"

namespace commons::window
{
	inline std::optional<std::string> getFocusedWindowTitle()
	{
		const HWND fgWindowHandle{GetForegroundWindow()};
		if (fgWindowHandle == nullptr)
		{
			std::print("Can't get focused window handle, or there is no focused window.\n");
			return std::nullopt;
		}

		const int titleLength{GetWindowTextLength(fgWindowHandle)};
		if (titleLength == 0)
		{
			std::print("Window title is 0 characters.\n");
			return std::nullopt;
		}

		std::vector<char> windowTitle(titleLength + 1);
		GetWindowText(fgWindowHandle, windowTitle.data(), titleLength + 1);

		return std::string(windowTitle.data());
	}

	inline bool isWindowInFocus(const std::string& windowName)
	{
		auto focusedWindowTitle = getFocusedWindowTitle();

		if (!focusedWindowTitle.has_value())
		{
			std::print("Focused window title is empty.\n");
			return false;
		}
		return focusedWindowTitle.value() == windowName;
	}
}
