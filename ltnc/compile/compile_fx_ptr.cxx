#include "compile.hxx"
#include "stdxx/functional.hxx"
#include "stdxx/iife.hxx"
#include <string_view>

namespace ltn::c {
	InstructionBuffer compile_expr(const sst::FxPointer & fx_ptr) {
		InstructionBuffer buf;
		buf << inst::newfx(fx_ptr.label.to_string(), fx_ptr.arity);
		return buf;
	}
}