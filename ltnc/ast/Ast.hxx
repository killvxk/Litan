#pragma once
#include "Node.hxx"
#include "Function.hxx"
#include "Expression.hxx"
#include "Statement.hxx"
#include "Literals.hxx"
#include "Assignable.hxx"

namespace ltn::c::ast {
	struct Program {
		std::vector<std::unique_ptr<Functional>> functions;
	};
}