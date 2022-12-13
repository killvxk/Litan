#include "compile.hxx"
#include "conversion.hxx"
#include <iostream>

namespace ltn::c {
	// compiles -> return...;
	StmtResult compile_stmt(
		const sst::Return & ret,
		CompilerInfo & info,
		Scope & scope) {
		
		InstructionBuffer buf;
		const auto return_type = scope.get_return_type();
		if(ret.expression) {
			auto code = compile_expression(*ret.expression, info, scope);
			buf << code.code;
			buf << conversion_on_return(code.deduced_type, return_type, ret.location);
		}
		else {
			buf << inst::null();
			buf << conversion_on_return(type::Null{}, return_type, ret.location);
		}


		// For returns from iife
		if(const auto returns_to = scope.get_return()) {
			buf << inst::jump(*returns_to);
		}
		else {
			buf << inst::retvrn();
		}
		return { buf, 0 };
	}
}