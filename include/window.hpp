#pragma once
#define WIN32_LEAN_AND_MEAN
#include <optional>
#include <string>
#include <vector>

#include "Windows.h"
#include "spdlog/spdlog.h"

namespace commons::window
{
	inline std::optional<std::string> getFocusedWindowTitle()
	{
		const HWND fgWindowHandle{GetForegroundWindow()};
		if (fgWindowHandle == nullptr)
		{
			spdlog::error("Can't get focused window handle, or there is no focused window.");
			return std::nullopt;
		}

		const int titleLength{GetWindowTextLength(fgWindowHandle)};
		if (titleLength == 0)
		{
			spdlog::error("Window title is 0 characters.");
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
			spdlog::error("Focused window title is empty.");
			return false;
		}
		return focusedWindowTitle.value() == windowName;
	}
}
