#include "CompilerInfo.hxx"

namespace ltn::c::compile {

	// creates a jump label
	std::string make_jump_id(const std::string_view name) {
		std::stringstream ss;
		ss << name << *stx::unique{};
		return ss.str();
	}
}