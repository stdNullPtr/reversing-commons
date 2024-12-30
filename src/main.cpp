﻿#include "window.hpp"
#include "xor.hpp"
#include <io.h>
#include <fcntl.h>
#include "logger.hpp"

using namespace commons;
using logger::LOG;

int main()
{
    _setmode(_fileno(stdout), _O_WTEXT);
    _setmode(_fileno(stdin), _O_WTEXT);

    std::wcout << XORW(L"=== START ===\n\n");
    std::wcout << XORW(L"=== СТАРТ ===\n\n");

    LOG(XORW(L"LOG: %s\n"), XORW(L"Hello, World!"));
    LOG(XORW(L"LOG: %s\n"), XORW(L"ЗДР, World!"));

    const auto focusedWindow{window::getFocusedWindowTitle()};
    std::wcout << XORW(L"window::getFocusedWindowTitle(): ") << focusedWindow.value_or(XORW(L"NO VALUE?")) << '\n';

    if (focusedWindow.has_value())
    {
        const auto isWindowInFocus{window::isWindowInFocus(focusedWindow.value())};
        std::wcout << XORW(L"window::isWindowInFocus(focusedWindow.value()): ") << (isWindowInFocus
            ? XORW(L"yes")
            : XORW(L"no"));
    }

    std::wcout << XORW(L"\n=== END ===\n");
    std::wcout << XORW(L"\n=== КРАЙ ===\n");

    return 0;
}
