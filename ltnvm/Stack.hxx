#pragma once
#include <vector>
#include <cstdint>
#include "Value.hxx"
namespace ltn::vm {
	class Stack {
	public:
		Value read(std::uint64_t offset) const;

		void write(std::uint64_t offset, Value value);

		void pushFrame(std::uint64_t localAlloc, std::uint64_t jumpBack);	
		std::uint64_t popFrame();
	private:

		std::vector<Value> stack;
		std::uint64_t framePointer = 0;
	};
}