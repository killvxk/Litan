#include "compiling.hxx"

namespace ltn::c::compile {
	namespace {
		using MT = ast::Modify::Type;
		
		// =
		ExprCode assign(
			const ast::Assign & expr,
			CompilerInfo & info,
			Scope & scope) {
			const auto l = compile::assignable(*expr.l, info, scope);
			const auto r = compile::expression(*expr.r, info, scope);
			std::stringstream ss;
			ss << r.code;
			ss << l.code;
			ss << inst::null;
			return ExprCode{ss.str() };
		}

		// += -= *= /= %=
		ExprCode modify(
			const ast::Modify & expr,
			CompilerInfo & info,
			Scope & scope) {
			const auto lW = compile::assignable(*expr.l, info, scope);
			const auto lR = compile::expression(*expr.l, info, scope);
			const auto r = compile::expression(*expr.r, info, scope);
			const auto op = [&] {
				switch (expr.type) {
				case MT::ADD: return inst::add;
				case MT::SUB: return inst::sub;
				case MT::MLT: return inst::mlt;
				case MT::DIV: return inst::div;
				case MT::MOD: return inst::mod;
				}
				throw CompilerError{
					"Unknow modify operator",
					expr.debugInfo.line};
			}();
			std::stringstream ss;
			ss << lR.code;
			ss << r.code;
			ss << op;
			ss << lW.code;			
			ss << inst::null;
			return ExprCode{ss.str() };
		}
	}

	// compiles any expression
	ExprCode expression(const ast::Expression & expr, CompilerInfo & info, Scope & scope) {
		if(auto binary = as<ast::Binary>(expr)) {
			return compile::binary(*binary, info, scope);
		}
		if(auto unary = as<ast::Unary>(expr)) {
			return compile::unary(*unary, info, scope);
		}
		if(auto primary = as<ast::Primary>(expr)) {
			return compile::primary(*primary, info, scope);
		}
		if(auto assign = as<ast::Assign>(expr)) {
			return compile::assign(*assign, info, scope);
		}
		if(auto modify = as<ast::Modify>(expr)) {
			return compile::modify(*modify, info, scope);
		}
		return { "...EXPR\n" };
	}
}