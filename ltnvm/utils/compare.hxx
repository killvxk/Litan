#pragma once
#include <stdexcept>
#include "ltnvm/Value.hxx"
#include "type_check.hxx"
namespace ltn::vm {
	struct VmCore;
	std::partial_ordering compare(const Value l, const Value r, VmCore & core);
	std::partial_ordering num_compare(const Value l, const Value r);
}