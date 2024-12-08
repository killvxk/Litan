#include "Tuple.hxx"
#include "ltnvm/native/native.hxx"
#include "ltnvm/Exception.hxx"

namespace ltn::vm {
	namespace {
		Value tuple_size(NativeCore *, const Value * args) {
			const Tuple * tuple = req_tuple(args + 0);
			return value::integer(std::size(*tuple));
		}



		Value tuple_is_empty(NativeCore *, const Value * args) {
			const Tuple * tuple = req_tuple(args + 0);
			return value::boolean(std::empty(*tuple));
		}



		Value tuple_at(NativeCore *, const Value * args) {
			const Tuple * tuple = req_tuple(args + 0);
			const std::int64_t index = req_int(args + 1);
			if(std::size(*tuple) <= index) {
				throw except::out_of_range();
			}
			if(0 > index) {
				throw except::out_of_range();
			}
			return tuple->unsafe_at(index);
		}



		Value tuple_front(NativeCore *, const Value * args) {
			const Tuple * tuple = req_tuple(args + 0);
			return tuple->unsafe_front();
		}



		Value tuple_back(NativeCore *, const Value * args) {
			const Tuple * tuple = req_tuple(args + 0);
			return tuple->unsafe_back();
		}



		NativeFunctionTable native_function_table {
			wrap(MemberCode::SIZE,     tuple_size,     1),
			wrap(MemberCode::IS_EMTPY, tuple_is_empty, 1),
			wrap(MemberCode::AT,       tuple_at,       2),
			wrap(MemberCode::FRONT,    tuple_front,    1),
			wrap(MemberCode::BACK,     tuple_back,     1),
		};
	}




	Value Tuple::get_member(std::uint64_t id) const {
		return search_native_function_table(native_function_table, id);
	}
}