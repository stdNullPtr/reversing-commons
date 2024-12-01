#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace commons::keyboard
{
    inline bool isKeyPressed(const int virtualKey)
    {
        return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
    }
}
