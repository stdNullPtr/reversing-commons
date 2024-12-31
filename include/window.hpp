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

    inline std::optional<std::wstring> get_focused_window_title()
    {
        const HWND fg_window_handle{GetForegroundWindow()};
        if (fg_window_handle == nullptr)
        {
            std::wcout << XORW(L"Can't get focused window handle, or there is no focused window.\n");
            return std::nullopt;
        }

        const int title_length{GetWindowTextLength(fg_window_handle)};
        if (title_length == 0)
        {
            std::wcout << XORW(L"Window title is 0 characters.\n");
            return std::nullopt;
        }

        const std::unique_ptr<wchar_t[]> buffer(new wchar_t[title_length + 1]);
        GetWindowText(fg_window_handle, buffer.get(), title_length + 1);

        return std::wstring(buffer.get());
    }

    inline bool is_window_in_focus(const std::wstring& window_name)
    {
        const auto focused_window_title{get_focused_window_title()};
        if (!focused_window_title.has_value())
        {
            std::wcout << XORW(L"Focused window title is empty.\n");
            return false;
        }
        return focused_window_title.value() == window_name;
    }

    inline HWND get_window_handle(const std::wstring& window_name)
    {
        return FindWindow(nullptr, window_name.c_str());
    }

    inline bool is_window_running(const std::wstring& window_name)
    {
        return get_window_handle(window_name) != nullptr;
    }

    //TODO why ::max() not working??
    inline bool wait_for_window(const std::wstring& window_name, const std::chrono::milliseconds& timeout = 999999h)
    {
        const auto start{std::chrono::steady_clock::now()};

        while (std::chrono::steady_clock::now() - start < timeout)
        {
            console::clear_console({0, 0});
            std::wcout << XORW(L"Looking for window '") << window_name << XORW(L"' Press (END to abort)...\n");

            if (is_window_running(window_name))
            {
                std::wcout << XORW(L"Window '") << window_name << XORW(L"' was found.\n");
                return true;
            }

            if (keyboard::is_key_pressed(VK_END))
            {
                std::wcout << XORW(L"Exit key pressed. Aborting looking for window '") << window_name << XORW(L"'.\n");
                return false;
            }

            std::this_thread::sleep_for(10ms);
        }

        std::wcout << XORW(L"Timeout reached; window '") << window_name << XORW(L"' was not found.\n");
        return false;
    }


}
