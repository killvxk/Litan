#pragma once
#include <array>
#include "ltnc/inst/instructions.hxx"
using namespace ltn::inst;
namespace ltn::c::build_in {
	const auto functional_invoke_2 = std::to_array<Instruction>({
		Invoke{},
		Return{},
	});

	const auto functional_invoke_1 = std::to_array<Instruction>({
		Newarr{0},
		Invoke{},
		Return{},
	});

	const auto functional_external = std::to_array<Instruction>({
		External{},
		Return{},
	});

	const auto functional_arity = std::to_array<Instruction>({
		BuildIn{0x20}, 
		Return{},
	});
}