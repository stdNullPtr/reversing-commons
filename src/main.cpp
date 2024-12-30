#include "window.hpp"
#include "xor.hpp"
#include <io.h>
#include <fcntl.h>
#include "logger.hpp"

using namespace commons;
using logger::log;

int main()
{
    _setmode(_fileno(stdout), _O_WTEXT);
    _setmode(_fileno(stdin), _O_WTEXT);

    std::wcout << XORW(L"=== START ===\n\n");
    std::wcout << XORW(L"=== СТАРТ ===\n\n");

    log(XORW(L"LOG: %s\n"), XORW(L"Hello, World!"));
    log(XORW(L"LOG: %s\n"), XORW(L"ЗДР, World!"));

    const auto focused_window{window::get_focused_window_title()};
    std::wcout << XORW(L"window::getFocusedWindowTitle(): ") << focused_window.value_or(XORW(L"NO VALUE?")) << '\n';

    if (focused_window.has_value())
    {
        const auto is_window_in_focus{window::is_window_in_focus(focused_window.value())};
        std::wcout << XORW(L"window::isWindowInFocus(focusedWindow.value()): ") << (is_window_in_focus? XORW(L"yes"): XORW(L"no"));
    }

    std::wcout << XORW(L"\n=== END ===\n");
    std::wcout << XORW(L"\n=== КРАЙ ===\n");

    return 0;
}
