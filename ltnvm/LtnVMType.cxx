#include "LtnVM.hxx"
#include "type_check.hxx"
#include <iostream>


namespace ltn::vm {
	void LtnVM::type_id() {
		const auto value = this->reg.pop();
		this->reg.push(Value{static_cast<std::int64_t>(value.type)});
	}

	void LtnVM::clone() {
		const auto ref = this->reg.pop();
		if(isObject(ref)) {
			const auto & newref = this->heap.clone(ref.u);
			this->reg.push(Value{newref, ref.type});
		}
		else throw except::invalid_argument();
	}
}