#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace commons::keyboard
{
    // Check if the key is currently pressed
    inline bool is_key_pressed(const int virtual_key)
    {
        return (GetAsyncKeyState(virtual_key) & 0x8000) != 0;
    }

    // Check if the key was pressed since the last check
    inline bool was_key_pressed(const int virtual_key)
    {
        return (GetAsyncKeyState(virtual_key) & 0x1) != 0;
    }
}
