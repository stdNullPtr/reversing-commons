#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "xor.hpp"

namespace commons::console
{
    namespace ansi_codes
    {
        constexpr auto red{"\x1B[31m"};
        constexpr auto light_blue{"\x1B[36m"};
        constexpr auto reset_color{"\033[0m"};
    }

    inline COORD get_cursor_position()
    {
        static const HANDLE h_console_output{GetStdHandle(STD_OUTPUT_HANDLE)};
        CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
        GetConsoleScreenBufferInfo(h_console_output, &screen_buffer_info);
        return screen_buffer_info.dwCursorPosition;
    }

    inline void set_cursor_position(const COORD& position)
    {
        static const HANDLE h_console_output{GetStdHandle(STD_OUTPUT_HANDLE)};
        SetConsoleCursorPosition(h_console_output, position);
    }

    inline void clear_console(const COORD& start_pos)
    {
        static const HANDLE h_console_output{GetStdHandle(STD_OUTPUT_HANDLE)};
        CONSOLE_SCREEN_BUFFER_INFO screen;
        static DWORD written;

        GetConsoleScreenBufferInfo(h_console_output, &screen);
        FillConsoleOutputCharacterA(h_console_output, ' ', screen.dwSize.X * screen.dwSize.Y, start_pos, &written);
        FillConsoleOutputAttribute(h_console_output, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, start_pos, &written);
        SetConsoleCursorPosition(h_console_output, start_pos);
    }

    inline void set_cursor_visibility(const bool& is_visible)
    {
        static const HANDLE h_console_output{GetStdHandle(STD_OUTPUT_HANDLE)};
        CONSOLE_CURSOR_INFO cursor_info;

        GetConsoleCursorInfo(h_console_output, &cursor_info);
        cursor_info.bVisible = is_visible;
        SetConsoleCursorInfo(h_console_output, &cursor_info);
    }

    inline bool init_console()
    {
        if (!AllocConsole())
        {
            return false;
        }

        set_cursor_visibility(false);

        (void)freopen_s(reinterpret_cast<FILE**>(stdin), XOR("CONIN$"), XOR("r"), stdin);
        (void)freopen_s(reinterpret_cast<FILE**>(stdout), XOR("CONOUT$"), XOR("w"), stdout);
        (void)freopen_s(reinterpret_cast<FILE**>(stderr), XOR("CONOUT$"), XOR("w"), stderr);

        return true;
    }

    inline void destroy_console()
    {
        const HWND h_console_output{GetConsoleWindow()};
        if (!h_console_output)
        {
            return;
        }

        FreeConsole();
        SendMessage(h_console_output, WM_CLOSE, 0, 0);
    }
}
