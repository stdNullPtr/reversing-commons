#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "xor.hpp"
#include <string>

namespace commons::console
{
    inline COORD getCursorPosition()
    {
        static const HANDLE hConsoleOutput{GetStdHandle(STD_OUTPUT_HANDLE)};
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
        GetConsoleScreenBufferInfo(hConsoleOutput, &screenBufferInfo);
        return screenBufferInfo.dwCursorPosition;
    }

    inline void setCursorPosition(const COORD& position)
    {
        static const HANDLE hConsoleOutput{GetStdHandle(STD_OUTPUT_HANDLE)};
        SetConsoleCursorPosition(hConsoleOutput, position);
    }

    inline void clearConsole(const COORD& startPos)
    {
        static const HANDLE hConsoleOutput{GetStdHandle(STD_OUTPUT_HANDLE)};
        CONSOLE_SCREEN_BUFFER_INFO screen;
        static DWORD written;

        GetConsoleScreenBufferInfo(hConsoleOutput, &screen);
        FillConsoleOutputCharacterA(hConsoleOutput, ' ', screen.dwSize.X * screen.dwSize.Y, startPos, &written);
        FillConsoleOutputAttribute(hConsoleOutput, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
                                   screen.dwSize.X * screen.dwSize.Y, startPos, &written);
        SetConsoleCursorPosition(hConsoleOutput, startPos);
    }

    inline void setCursorVisibility(const bool& isVisible)
    {
        static const HANDLE hConsoleOutput{GetStdHandle(STD_OUTPUT_HANDLE)};
        CONSOLE_CURSOR_INFO cursorInfo;

        GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
        cursorInfo.bVisible = isVisible;
        SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
    }

    inline bool initConsole()
    {
        if (!AllocConsole())
        {
            return false;
        }

        setCursorVisibility(false);

        (void)freopen_s(reinterpret_cast<FILE**>(stdin), XOR("CONIN$"), XOR("r"), stdin);
        (void)freopen_s(reinterpret_cast<FILE**>(stdout), XOR("CONOUT$"), XOR("w"), stdout);
        (void)freopen_s(reinterpret_cast<FILE**>(stderr), XOR("CONOUT$"), XOR("w"), stderr);

        return true;
    }

    inline void destroyConsole()
    {
        const HWND hConsoleOutput{GetConsoleWindow()};
        if (!hConsoleOutput)
        {
            return;
        }

        FreeConsole();
        SendMessage(hConsoleOutput, WM_CLOSE, 0, 0);
    }
}
