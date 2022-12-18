#include "analyze.hxx"
#include <string_view>
namespace ltn::c {
	// compiles index read operation
	sst::expr_ptr analyze_expr(
		const ast::Index & index,
		Context & context,
		Scope & scope) {

		auto arr = analyze_expression(*index.expression, context, scope);
		auto idx = analyze_expression(*index.index, context, scope);
		
		const auto type = type::deduce_index(arr->type, idx->type);

		return std::make_unique<sst::Index>(
			std::move(arr),
			std::move(idx),
			type
		);
	}
}