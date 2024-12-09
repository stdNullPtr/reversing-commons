#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "Windows.h"
#include <optional>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <keyboard.hpp>
#include <xor.hpp>
#include <console.hpp>
#include <iostream>

namespace commons::window
{
    using namespace std::chrono_literals;

    inline std::optional<std::wstring> getFocusedWindowTitle()
    {
        const HWND fgWindowHandle{GetForegroundWindow()};
        if (fgWindowHandle == nullptr)
        {
            std::wcout << XORW(L"Can't get focused window handle, or there is no focused window.\n");
            return std::nullopt;
        }

        const int titleLength{GetWindowTextLength(fgWindowHandle)};
        if (titleLength == 0)
        {
            std::wcout << XORW(L"Window title is 0 characters.\n");
            return std::nullopt;
        }

        std::unique_ptr<wchar_t[]> buffer(new wchar_t[titleLength + 1]);
        GetWindowText(fgWindowHandle, buffer.get(), titleLength + 1);

        return std::wstring(buffer.get());
    }

    inline bool isWindowInFocus(const std::wstring& windowName)
    {
        const auto focusedWindowTitle{getFocusedWindowTitle()};

        if (!focusedWindowTitle.has_value())
        {
            std::wcout << XORW(L"Focused window title is empty.\n");
            return false;
        }
        return focusedWindowTitle.value() == windowName;
    }

    inline bool isWindowRunning(const std::wstring& windowName)
    {
        return FindWindow(nullptr, windowName.c_str()) != nullptr;
    }

    //TODO why ::max() not working??
    inline bool waitForWindow(const std::wstring& windowName, const std::chrono::milliseconds& timeout = 999999h)
    {
        const auto start{std::chrono::steady_clock::now()};

        while (std::chrono::steady_clock::now() - start < timeout)
        {
            console::clearConsole({0, 0});
            std::wcout << XORW(L"Looking for window '") << windowName << XORW(L"' Press (END to abort)...\n");

            if (isWindowRunning(windowName))
            {
                std::wcout << XORW(L"Window '") << windowName << XORW(L"' was found.\n");
                return true;
            }

            if (keyboard::isKeyPressed(VK_END))
            {
                std::wcout << XORW(L"Exit key pressed. Aborting looking for window '") << windowName << XORW(L"'.\n");
                return false;
            }

            std::this_thread::sleep_for(50ms);
        }

        std::wcout << XORW(L"Timeout reached; window '") << windowName << XORW(L"' was not found.\n");
        return false;
    }
}
