#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace commons::keyboard
{
    inline bool is_key_pressed(const int virtual_key)
    {
        return (GetAsyncKeyState(virtual_key) & 0x8000) != 0;
    }
}
