#pragma once
#include "Config.hxx"
#include "CompilerError.hxx"
#include "Reporter.hxx"
#include "Source.hxx"
#include "ltn/InstructionSet.hxx"
#include "optimize/optimize.hxx"
#include "print/print.hxx"
#include "peephole/peephole.hxx"

namespace ltn::c {
	ast::Program parse(
		std::vector<Source> sources,
		const Config & config,
		Reporter & reporter);

	std::vector<ltn::inst::Instruction> compile(
		const ast::Program & program,
		const Config & config,
		Reporter & reporter);
}