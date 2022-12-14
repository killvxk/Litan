#include "compile.hxx"
#include "ltnc/type/check.hxx"
#include <iostream>
#include "stdxx/functional.hxx"

namespace ltn::c {
	type::Type deduce_type(
		sst::Binary::Op op,
		const type::Type & l,
		const type::Type & r) {
		using OP = sst::Binary::Op;
		switch (op) {
			case OP::ADD:          return type::deduce_add(l, r);
			case OP::SUB:          return type::deduce_sub(l, r);
			case OP::MLT:          return type::deduce_mlt(l, r);
			case OP::DIV:          return type::deduce_div(l, r);
			case OP::MOD:          return type::deduce_mod(l, r);
			case OP::POW:          return type::deduce_pow(l, r);
			case OP::SMALLER:      return type::deduce_compare(l, r);
			case OP::BIGGER:       return type::deduce_compare(l, r);
			case OP::SMALLEREQUAL: return type::deduce_compare(l, r);
			case OP::BIGGEREQUAL:  return type::deduce_compare(l, r);
			case OP::EQUAL:        return type::deduce_compare(l, r);
			case OP::UNEQUEL:      return type::deduce_compare(l, r);
			case OP::SPACE_SHIP:   return type::deduce_three_way(l, r);
			case OP::SHIFT_L:      return type::deduce_bitwise(l, r);
			case OP::SHIFT_R:      return type::deduce_bitwise(l, r);
			case OP::BIT_AND:      return type::deduce_bitwise(l, r);
			case OP::BIT_OR:       return type::deduce_bitwise(l, r);
			case OP::BIT_XOR:      return type::deduce_bitwise(l, r);
			case OP::AND:          return type::deduce_logic(l, r);
			case OP::OR:           return type::deduce_logic(l, r);
			case OP::ELVIS:        return type::deduce_elvis(l, r);
			case OP::NULLCO:       return type::deduce_nullco(l, r);
		}
	}



	// compiles a binary operation
	sst::expr_ptr analyze_expr(
		const ast::Binary & binary,
		CompilerInfo & info,
		Scope & scope) {
		
		auto l = analyze_expression(*binary.l, info, scope);
		auto r = analyze_expression(*binary.r, info, scope);
		const auto op = static_cast<sst::Binary::Op>(binary.type);
		const auto type = deduce_type(op, l->type, r->type);

		return std::make_unique<sst::Binary>(op, std::move(l), std::move(r));
	}
	
}