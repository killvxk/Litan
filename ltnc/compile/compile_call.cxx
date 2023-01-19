#include "compile.hxx"
#include "stdxx/functional.hxx"
#include <string_view>

namespace ltn::c {
	// compiles function call fx(...)
	InstructionBuffer compile_expr(const sst::Invoke & invoke) {
		InstructionBuffer buf;

		buf << compile_expression(*invoke.function_ptr);

		for(const auto & param : invoke.parameters) {
			buf << compile_expression(*param);
		}

		buf << inst::newarr(invoke.parameters.size());
		buf << inst::invoke();

		return buf;
	}



	InstructionBuffer compile_expr(const sst::Call & call) {
		InstructionBuffer buf;
		const auto arity = call.parameters.size();
		for(std::size_t i = 0; i < arity; ++i) {
			buf << compile_expression(*call.parameters[i]);
		}
		buf << inst::call(call.label.to_string(), arity);		
		return buf;
	}
}