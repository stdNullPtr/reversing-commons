#include "lookup.hpp"
#include <spdlog/spdlog.h>

#include "xor.hpp"

using namespace commons;

int main()
{
	spdlog::info(XOR("START"));

	const auto focusedWindow{lookup::getFocusedWindowTitle()};
	spdlog::info(XOR("Focused window: {}"), focusedWindow.value_or("NO VALUE?"));

	if (focusedWindow.has_value())
	{
		const auto isWindowInFocus{lookup::isWindowInFocus(focusedWindow.value())};
		spdlog::info(XOR("isWindowInFocus: {}"), isWindowInFocus ? "yes" : "no");
	}

	spdlog::info(XOR("END"));

	return 0;
}
