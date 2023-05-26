#pragma once
#include "ltnvm/Heap.hxx"
#include "ltnvm/Value.hxx"

namespace ltn::vm::ext {
	inline Value wrap_return(bool value, Heap &) {
		return value::boolean(value);
	}
		
	inline Value wrap_return(std::integral auto value, Heap &) {
		return value::integer(value);
	}
		
	inline Value wrap_return(std::floating_point auto value, Heap &) {
		return value::floating(value);
	}

	inline Value wrap_return(std::string value, Heap & heap) {
		auto ptr = heap.alloc<String>(std::move(value));
		return value::string(ptr);
	}

	template <typename T>
	inline Value wrap_return(const std::optional<T> & value, Heap & heap) {
		if(value) {
			return wrap_return(value.value(), heap);
		}
		else {
			return value::null;
		}
	}
	
	template <typename T>
	inline Value wrap_return(const std::vector<T> & vector, Heap & heap) {
		auto ptr = heap.alloc<Array>({});
		auto & array = heap.read<Array>(ptr);
		for(const T & elem : vector) {
			array.push_back(wrap_return(elem, heap));
		} 
		return value::array(ptr);
	}
}