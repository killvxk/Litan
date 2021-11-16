#include "compiling.hxx"

namespace ltn::c::compile {
	
	StmtCode block(const ast::Block & block, CompilerInfo & info) {
		std::stringstream ss;
		for(const auto & stmt : block.statements) {
			if(stmt) {
				ss << statement(*stmt, info).code;
			} 
		}
		return {ss.str()};
	}

	StmtCode statement(const ast::Statement & stmt, CompilerInfo & info) {
		if(auto block = as<ast::Block>(stmt)) {
			return compile::block(*block, info);
		}
		if(auto exprstmt = as<ast::StatementExpression>(stmt)) {
			return { expression(*exprstmt->expression, info).code };
		}
		return { "...\n" };
	}
}