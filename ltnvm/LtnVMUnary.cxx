#include "LtnVM.hxx"
#include "TypeCheck.hxx"
#include "convert.hxx"


namespace ltn::vm {
	#define FETCH const auto x = this->reg.pop();

	void LtnVM::neg() {
		FETCH
		if(isBool(x)) {
			return this->reg.push({- static_cast<std::int64_t>(x.b)});
		}
		if(isChar(x)) {
			return this->reg.push({- static_cast<std::int64_t>(x.c)});
		}
		if(isInt(x)) {
			return this->reg.push({- x.i});
		}
		if(isFloat(x)) {
			return this->reg.push({- x.f});
		}
		throw except::invalidArgument();
	}
	void LtnVM::n0t() {
		FETCH
		return this->reg.push(!convert::to_bool(x));
	}

	void LtnVM::inc() {
		FETCH
		if(isInt(x)) {
			return this->reg.push({x.i + 1});
		}
		if(isFloat(x)) {
			return this->reg.push({x.f + 1});
		}
		throw except::invalidArgument();
	}
	void LtnVM::dec() {
		FETCH
		if(isInt(x)) {
			return this->reg.push({x.i - 1});
		}
		if(isFloat(x)) {
			return this->reg.push({x.f - 1});
		}
		throw except::invalidArgument();
	}

	#undef FETCH
}