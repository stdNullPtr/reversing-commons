#include "window.hpp"
#include "xor.hpp"
#include <print>

using namespace commons;

int main()
{
	std::println(XOR("=== START ===\n"));

	const auto focusedWindow{window::getFocusedWindowTitle()};
	std::println(XOR("window::getFocusedWindowTitle(): {}"), focusedWindow.value_or("NO VALUE?"));

	if (focusedWindow.has_value())
	{
		const auto isWindowInFocus{window::isWindowInFocus(focusedWindow.value())};
		std::println(XOR("window::isWindowInFocus(focusedWindow.value()): {}"), isWindowInFocus ? "yes" : "no");
	}

	std::println(XOR("\n=== END ==="));

	return 0;
}
