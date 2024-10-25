#include "window.hpp"
#include "xor.hpp"
#include "spdlog/spdlog.h"

using namespace commons;

int main()
{
	spdlog::info(XOR("START"));

	const auto focusedWindow{window::getFocusedWindowTitle()};
	spdlog::info(XOR("Focused window: {}"), focusedWindow.value_or("NO VALUE?"));

	if (focusedWindow.has_value())
	{
		const auto isWindowInFocus{window::isWindowInFocus(focusedWindow.value())};
		spdlog::info(XOR("isWindowInFocus: {}"), isWindowInFocus ? "yes" : "no");
	}

	spdlog::info(XOR("END"));

	return 0;
}
