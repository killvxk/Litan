#include "compile.hxx"
#include <string_view>
namespace ltn::c {
	namespace {
		sst::expr_ptr analyze_read_local_variable(const Variable & var) {
			InstructionBuffer buf;
			buf << inst::read_x(var.address);
			return sst::expr_ptr{ 
				.code = buf,
				.deduced_type = var.type
			};
		}



		bool is_inner_namespace(
			const Namespace & call_ns,
			const Namespace & fx_ns) {
			if(fx_ns.size() > call_ns.size()) return false;
			for(std::size_t i = 0; i < fx_ns.size(); i++) {
				if(call_ns[i] != fx_ns[i]) {
					return false;
				}
			}
			return true;
		}
	}



	void guard_private(
		const ast::Functional & fx,
		const Namespace & call_ns,
		const SourceLocation & loc) {
		if(
			fx.pr1vate &&
			!is_inner_namespace(call_ns, fx.namespaze)) {
			throw CompilerError {
				"Function is not visible in current scope",
				loc
			};
		}
	}



	// compiles an variable read accessc
	sst::expr_ptr analyze_expr(
		const ast::Var & expr,
		CompilerInfo & info,
		Scope & scope) {

		const auto & name = expr.name;
		const auto & namespaze = scope.get_namespace();
		
		const auto * var = scope.resolve(expr.name, expr.location);
		if(var && expr.namespaze.empty()) {
			return analyze_read_local_variable(*var);
		}
		
		const auto * def = info.definition_table.resolve(
			name,
			namespaze,
			expr.namespaze);
		
		if(def) {
			InstructionBuffer buf;
			buf << inst::global_read(def->id);
			return sst::expr_ptr{ buf };
		}
		
		throw CompilerError {
			"Undefined variable " + expr.namespaze.to_string() + name,
			expr.location
		};
	}

	
	sst::expr_ptr analyze_expr(
		const ast::Member & access,
		CompilerInfo & info,
		Scope & scope) {

		InstructionBuffer buf;
		buf << analyze_expression(*access.expr, info, scope).code;
		const auto id = info.member_table.get_id(access.name);
		buf << inst::member_read(id);
		return sst::expr_ptr{ buf };
	}
}