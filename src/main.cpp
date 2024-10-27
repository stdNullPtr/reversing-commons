#include "window.hpp"
#include "xor.hpp"
#include <print>

using namespace commons;

int main()
{
	std::print(XOR("START\n\n"));

	const auto focusedWindow{window::getFocusedWindowTitle()};
	std::print(XOR("Focused window: {}\n"), focusedWindow.value_or("NO VALUE?"));

	if (focusedWindow.has_value())
	{
		const auto isWindowInFocus{window::isWindowInFocus(focusedWindow.value())};
		std::print(XOR("isWindowInFocus: {}\n"), isWindowInFocus ? "yes" : "no");
	}

	std::print(XOR("END\n\n"));

	return 0;
}
