#include "compile.hxx"

namespace ltn::c {
	std::string jump_begin(const std::string & name) {
		return name + "_BEGIN";
	}

	std::string jump_end(const std::string & name) {
		return name + "_END";
	}

	std::string var_from(const std::string & name) {
		return name + "_FROM";
	}

	std::string var_to(const std::string & name) {
		return name + "_TO";
	}

	sst::stmt_ptr analyze_stmt(
		const ast::While & stmt,
		CompilerInfo & info,
		Scope & scope) {

		// outer scope of loop 
		MinorScope loop_scope { &scope }; 
		
		// compile parts
		const auto condition = analyze_expression(*stmt.condition, info, scope);
		const auto body = analyze_statement(*stmt.body, info, loop_scope);

		return std::make_unique<sst::While>(
			body->local_vars,
			false,
			std::move(condition),
			std::move(body)
		);
	}



	sst::stmt_ptr analyze_stmt(
		const ast::InfiniteLoop & stmt,
		CompilerInfo & info,
		Scope & scope) {

		MinorScope loop_scope { &scope }; 		
		const auto body = analyze_statement(*stmt.body, info, loop_scope);
		return std::make_unique<sst::InfiniteLoop>(
			body->local_vars,
			false,
			std::move(body)
		);
	}



	sst::stmt_ptr analyze_stmt(
		const ast::For & stmt,
		CompilerInfo & info,
		Scope & scope) {
		
		// outer scope of loop 
		MinorScope loop_scope { &scope };

		auto var = analyze_statement(*stmt.var, info, loop_scope);
		auto from = analyze_expression(*stmt.from, info, loop_scope);
		auto to = analyze_expression(*stmt.to, info, loop_scope);
		auto step = stmt.step ? analyze_expression(*stmt.step, info, scope) : nullptr;
		
		const auto label = make_jump_id("FOR");

		const auto i_var    = loop_scope.resolve(stmt.var->name, stmt.location);
		const auto from_var = loop_scope.insert(var_from(label), stmt.location);
		const auto to_var   = loop_scope.insert(var_to(label), stmt.location);

		const auto body = analyze_statement(*stmt.body, info, loop_scope);
				
		return std::make_unique<sst::For>(
			body->local_vars + 3, false,
			label,
			std::move(var),
			std::move(from),
			std::move(to),
			std::move(step),
			std::move(body)
		);
	}
}