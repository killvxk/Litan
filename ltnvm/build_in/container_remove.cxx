#include "algorithm.hxx"
#include "ltnvm/utils/convert.hxx"
#include "ltnvm/Exception.hxx"
#include "ltnvm/utils/index.hxx"
#include "ltnvm/inst/instructions.hxx"
#include "ltnvm/utils/stringify.hxx"

namespace ltn::vm::build_in {
	namespace {
		using array = Array;
		using string = std::string;



		void guardEmpty(const auto & collection) {
			if(std::empty(collection)) throw except::empty_collection();
		}

		

		template<typename Collection>
		Value remove_first(const Value ref) {
			Collection * collection = value::as<Collection>(ref); 
			guardEmpty(*collection);
			collection->unsafe_erase(collection->begin());
			return value::null;
		}



		template<typename Collection>
		Value remove_last(const Value ref) {
			Collection * collection = value::as<Collection>(ref); 
			guardEmpty(*collection);
			collection->unsafe_pop_back();
			return value::null;
		}



		template<typename Collection>
		Value remove_index(const Value ref, const Value & index) {
			Collection * collection = value::as<Collection>(ref);
			const auto i = to_index(index, std::ssize(*collection));
			collection->unsafe_erase(collection->begin() + i);
			return value::null;
		}



		Value remove_map(const Value ref, const Value key) {
			Map * map = value::as<Map>(ref); 
			map->erase(key);
			return value::null;
		}
	}

	

	Value remove_front(VMCore & core) {
		const auto ref = core.stack.pop();
		if(is_array(ref)) return remove_first<Array>(ref);
		throw except::invalid_argument();
	}



	Value remove_back(VMCore & core) {
		const auto ref = core.stack.pop();
		if(is_array(ref)) return remove_last<Array>(ref);
		throw except::invalid_argument();
	}



	Value remove(VMCore & core) {
		const auto key = core.stack.pop();
		const auto ref = core.stack.pop();
		if(is_map(ref)) return remove_map(ref, key);
		if(is_array(ref)) return remove_index<Array>(ref, key);
		throw except::invalid_argument();
	}
}