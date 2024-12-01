#include "window.hpp"
#include "xor.hpp"

using namespace commons;

int main()
{
    std::cout << (XOR("=== START ===\n"));

    const auto focusedWindow{window::getFocusedWindowTitle()};
    std::cout << XOR("window::getFocusedWindowTitle(): ") << focusedWindow.value_or(XOR("NO VALUE?")) << '\n';

    if (focusedWindow.has_value())
    {
        const auto isWindowInFocus{window::isWindowInFocus(focusedWindow.value())};
        std::cout << XOR("window::isWindowInFocus(focusedWindow.value()): ") << (isWindowInFocus ? XOR("yes") : XOR("no"));
    }

    std::cout << XOR("\n=== END ===\n");
    XorCompileTime::w_printf(XOR("testtttt\n"));

    return 0;
}
