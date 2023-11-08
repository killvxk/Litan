#pragma once
#include <vector>
#include "Expression.hxx"
#include "ltnc/Label.hxx"

namespace ltn::c::sst {
	struct FxPointer final : public ExpressionCRTP<FxPointer> {
		FxPointer(
			const Label & label,
			std::size_t arity,
			bool is_variadic);

		virtual ~FxPointer();

		virtual std::uint64_t alloc() const override;

		Label label;
		std::size_t arity;
		std::vector<std::unique_ptr<Var>> captures;
		bool is_variadic;
	};

	std::unique_ptr<FxPointer> fx_pointer(const Label & label, std::size_t arity, bool is_variadic);
}