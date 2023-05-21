#pragma once
#include <cstdint>
#include <vector>
#include "ltnvm/memory/Value.hxx"

namespace ltn::vm {
	struct VmCore;
	class Heap;
}

namespace ltn::vm {
	class CombinedIterator {
	public:
		CombinedIterator(std::vector<std::uint64_t> iters);
		Value next(Heap &);
		Value get(Heap &);
		void move(Heap &, std::int64_t amount);
		void mark(Heap &);
	private:
		std::vector<std::uint64_t> iters;
	};
}