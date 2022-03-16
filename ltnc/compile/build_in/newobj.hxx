#pragma once
#include <array>
#include <string_view>
using namespace std::string_view_literals;
namespace ltn::c::compile::build_in {
	constexpr auto queue = std::array{
		"newqueue"sv,
		"return"sv,
	};

	constexpr auto stack = std::array{
		"newstack"sv,
		"return"sv,
	};

	constexpr auto map = std::array{
		"newmap"sv,
		"return"sv,
	};

	constexpr auto array = std::array{
		"newarr 0"sv,
		"return"sv,
	};

	constexpr auto range = std::array{
		"newrange"sv,
		"return"sv,
	};

	constexpr auto struc1 = std::array{
		"newstruct"sv,
		"return"sv,
	};
}