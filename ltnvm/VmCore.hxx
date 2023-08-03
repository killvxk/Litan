#pragma once
#include <unordered_map>
#include "ltnvm/Heap.hxx"
#include "ltnvm/VmStack.hxx"
#include "ltnvm/TypeTable.hxx"
#include "external/Callable.hxx"

namespace ltn::vm {
	struct VmCore {
		// Runtime
		VmStack stack;
		Heap heap;
		const std::uint8_t * pc;
		const std::uint8_t * code_begin;
		const std::uint8_t * code_end;

		std::unordered_map<std::int64_t, ext::Callable> fx_table_ltn_to_cxx;
		std::unordered_map<std::string, std::uint64_t> fx_table_cxx_to_ltn;
		std::unordered_map<std::string, std::uint64_t> static_table;
		std::unordered_map<std::uint64_t, std::string> member_name_table;

		TypeTable type_table;
		
		inline std::uint8_t fetch_byte() {
			return *this->pc++;
		}
	

		inline std::uint64_t fetch_uint() {
			std::uint64_t value = 0;
			for(auto i = 0; i < 8; i++) {
				value <<= 8;
				value |= static_cast<std::uint64_t>(*this->pc++);
			}
			return value;
		}

			
		inline const std::uint8_t * fetch_str() {
			return this->pc;
		}
	};
}