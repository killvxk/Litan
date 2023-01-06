#include "instructions.hxx"
#include <cmath>
#include "ltnvm/type_check.hxx"
#include <sstream>
#include "ltnvm/Operations.hxx"
#include "ltnvm/calcBinary.hxx"
#include "ltnvm/convert.hxx"
#include "ltnvm/cast.hxx"

#define FETCH\
	const auto r = core.reg.pop();\
	const auto l = core.reg.pop();

namespace ltn::vm::inst {
	namespace {
		template<class T>
		std::vector<T> operator+(
			const std::vector<T> & l,
			const std::vector<T> & r) {
			std::vector<T> vec;
			vec.reserve(l.size() + r.size());
			vec.insert(std::end(vec), l.begin(), l.end());
			vec.insert(std::end(vec), r.begin(), r.end());
			return vec;
		}


		Array toArray(const Value & value, Heap & heap) {
			if(is_array(value)) {
				return heap.read<Array>(value.u);
			}
			return {{value}};
		}



		std::string repeat(const std::string & str, const std::integral auto count) {
			std::ostringstream oss;
			for(std::int64_t i = 0; i < count; ++i) {
				oss << str;
			}
			return oss.str();
		}



		Value string_repetition(
			const Value & str_ref,
			const Value & repetitions,
			Heap & heap) {
			
			const auto & str = heap.read<String>(str_ref.u).get();
			const auto & count = cast::to_int(repetitions);
			auto repeated = repeat(str, count);
			const auto ptr = heap.alloc<String>({std::move(repeated)}); 
			return value::string(ptr);
		}
	}



	void add(VmCore & core) {
		FETCH
		
		if(is_array(l) && is_array(r)) {
			const auto arr_l = toArray(l, core.heap).get();
			const auto arr_r = toArray(r, core.heap).get();
			const auto ref = core.heap.alloc<Array>({arr_l + arr_r});
			return core.reg.push(value::array(ref));
		}

		if(is_string(l) && is_string(r)) {
			const auto str_l = convert::to_string(l, core.heap);
			const auto str_r = convert::to_string(r, core.heap);
			const auto ref = core.heap.alloc<String>({str_l + str_r});
			return core.reg.push(value::string(ref));
		}

		core.reg.push(calc<Addition>(l, r));
	}



	void sub(VmCore & core) {
		FETCH
		core.reg.push(calc<Subtraction>(l, r));
	}



	void mlt(VmCore & core) {
		FETCH

		if(is_string(l) && is_integral(r)) {
			return core.reg.push(string_repetition(l, r, core.heap));
		}

		if(is_integral(l) && is_string(r)) {
			return core.reg.push(string_repetition(r, l, core.heap));
		}

		core.reg.push(calc<Multiplication>(l, r));
	}



	void div(VmCore & core) {
		FETCH
		core.reg.push(calc<Division>(l, r));
	}



	void mod(VmCore & core) {
		FETCH
		core.reg.push(calc<Modulo>(l, r));
	}



	void pow(VmCore & core) {
		FETCH
		core.reg.push(calc<Power>(l, r));
	}
}
#undef FETCH