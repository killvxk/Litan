#pragma once
#include <vector>
#include <string_view>
#include "ltnvm/Value.hxx"
namespace ltn::vm {
	using Array = std::vector<Value>;

	inline Array clone(const Array & arr) {
		return arr;
	}
}