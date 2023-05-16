#include "type.hxx"
#include "ltnvm/type_check.hxx"
#include "ltnvm/convert.hxx"
#include "ltnvm/cast.hxx"
#include "ltnvm/VmCore.hxx"
#include "ltn/type_code.hxx"
#include "stdxx/string_literal.hxx"

namespace ltn::vm {
	namespace {
		using TypeResult = std::pair<const TypeNode*, const std::uint8_t*>;
		inline std::uint64_t read_uint_64(const std::uint8_t * code) {
			std::uint64_t size = 0;
			for(std::size_t i = 0; i < 8; ++i) {
				size += static_cast<std::uint64_t>(*(code++)) << (i * 8);
			}
			return size;
		}



		bool is_array_or_tuple(const Value & value) {
			return is_array(value) || is_tuple(value);
		}



		template<stx::string_literal NAME, auto IS, auto CAST>
		class PrimaryTypeNode : public TypeNode {
		public:
			virtual bool is(const Value & value, Heap & heap) const override {
				return IS(value, heap);
			}

			virtual Value cast(const Value & value, Heap & heap) const override {
				return CAST(value, heap);
			}

			virtual std::string name() const override {
				return NAME.str();
			}
		};



		template<stx::string_literal NAME, auto IS, auto CAST>
		class UnaryTypeNode : public TypeNode {
		public:
			UnaryTypeNode(const TypeNode * sub_type) : sub_type{sub_type} {}

			virtual bool is(const Value & value, Heap & heap) const override {
				return IS(value, heap, sub_type);
			}

			virtual Value cast(const Value & value, Heap & heap) const override {
				return CAST(value, heap, sub_type);
			}

			virtual std::string name() const override {
				return NAME.str() + "(" + sub_type->name() + ")";
			}

		private:
			const TypeNode * sub_type;
		};




		template<stx::string_literal NAME, auto IS, auto CAST>
		class BinaryTypeNode : public TypeNode {
		public:
			BinaryTypeNode(
				const TypeNode * sub_type_l,
				const TypeNode * sub_type_r)
				: sub_type_l{sub_type_l}
				, sub_type_r{sub_type_r} {}

			virtual bool is(const Value & value, Heap & heap) const override {
				return IS(value, heap, sub_type_l, sub_type_r);
			}

			virtual Value cast(const Value & value, Heap & heap) const override {
				return CAST(value, heap, sub_type_l, sub_type_r);
			}

			virtual std::string name() const override {
				return NAME.str() + "(" + sub_type_l->name() + "," + sub_type_r->name() + ")";
			}

		private:
			const TypeNode * sub_type_l;
			const TypeNode * sub_type_r;
		};



		template<stx::string_literal NAME, auto IS, auto CAST>
		class NAryTypeNode : public TypeNode {
		public:
			NAryTypeNode(std::vector<const TypeNode *> sub_types)
				: sub_types{std::move(sub_types)} {}

			virtual bool is(const Value & value, Heap & heap) const override {
				return IS(value, heap, sub_types);
			}

			virtual Value cast(const Value & value, Heap & heap) const override {
				return CAST(value, heap, sub_types);
			}

			virtual std::string name() const override {
				std::ostringstream oss;
				oss << NAME.str() << "(";
				bool first = true;
				for(const auto & st : this->sub_types) {
				 	if(first) first = false;
					else oss << ",";
					oss << st->name();
				}
				oss << ")";
				return oss.str();
			}

		private:
			std::vector<const TypeNode *> sub_types;
		};



		template<stx::string_literal NAME, auto IS, auto CAST>
		class NumericTypeNode : public TypeNode {
		public:
			NumericTypeNode(std::uint64_t number) : number{number} {}

			virtual bool is(const Value & value, Heap & heap) const override {
				return IS(value, heap, number);
			}

			virtual Value cast(const Value & value, Heap & heap) const override {
				return CAST(value, heap, number);
			}

			virtual std::string name() const override {
				std::ostringstream oss;
				oss << NAME.str() << "(" << number << ")";
				return oss.str();
			}

		private:
			std::uint64_t number;
		};



		template<auto IS>
		bool simple_check_for(const Value & value, Heap &) {
			return IS(value);
		}


		
		template<auto IS>
		Value ret_if_or_null(const Value & value, Heap & heap) {
			return IS(value) ? value : value::null;
		}



		Value no_cast(const Value & value, Heap &) {
			return value;
		}



		bool always_true(const Value &, Heap &) {
			return true;
		}



		Value type_cast_null(const Value & value, Heap &) {
			return value::null;
		}



		Value type_cast_bool(const Value & value, Heap &) {
			return value::boolean(cast::to_bool(value));
		}


		
		Value type_cast_char(const Value & value, Heap &) {
			return value::character(cast::to_char(value));
		}



		Value type_cast_int(const Value & value, Heap &) {
			return value::integer(cast::to_int(value));
		}



		Value type_cast_float(const Value & value, Heap & heap) {
			return value::floating(cast::to_float(value, heap));
		}



		Value type_cast_string(const Value & value, Heap & heap) {
			return value::string(heap.alloc(cast::to_string(value, heap)));
		}



		template<typename Data, auto make_value, auto check_type>
		Value cast_unary_type(const Value & value, Heap & heap, const TypeNode * sub_type) {
			if(!check_type(value)) return value::null;
			const Data & input = heap.read<Data>(value); 
			Data output;
			for(const auto & elem : input) {
				output.push_back(sub_type->cast(elem, heap));
			}
			return make_value(heap.alloc(std::move(output)));
		}



		Value type_cast_array(const Value & value, Heap & heap, const TypeNode * sub_type) {
			return cast_unary_type<Array, value::array, is_array>(value, heap, sub_type);
		}


		
		Value type_cast_queue(const Value & value, Heap & heap, const TypeNode * sub_type) {
			return cast_unary_type<Deque, value::queue, is_queue>(value, heap, sub_type);
		}


		
		Value type_cast_stack(const Value & value, Heap & heap, const TypeNode * sub_type) {
			return cast_unary_type<Deque, value::stack, is_stack>(value, heap, sub_type);
		}



		template<typename Data, auto check_type>
		bool is_unary_type(const Value & value, Heap & heap, const TypeNode * sub_type) {
			if(!check_type(value)) return false;
			const Data & input = heap.read<Data>(value);
			return std::all_of(std::begin(input), std::end(input), 
				[&](const auto & elem) { return sub_type->is(elem, heap); 
			});
		}



		bool type_is_array(const Value & value, Heap & heap, const TypeNode * subtype) {
			return is_unary_type<Array, is_array>(value, heap, subtype);
		}



		bool type_is_queue(const Value & value, Heap & heap, const TypeNode * subtype) {
			return is_unary_type<Deque, is_queue>(value, heap, subtype);
		}


		
		bool type_is_stack(const Value & value, Heap & heap, const TypeNode * subtype) {
			return is_unary_type<Deque, is_stack>(value, heap, subtype);
		}



		bool type_is_fx_n(const Value & value, Heap & heap, std::uint64_t arity) {
			if(!is_fxptr(value)) return false;
			const FxPointer & input = heap.read<FxPointer>(value);
			return input.params == arity;
		}



		Value type_cast_fx_n(const Value & value, Heap & heap, std::uint64_t arity) {
			return type_is_fx_n(value, heap, arity) ? value : value::null; 
		}



		bool type_is_map(const Value & value, Heap & heap, const TypeNode * sub_type_l, const TypeNode * sub_type_r) {
			if(!is_map(value)) return false;
			const Map & input = heap.read<Map>(value);
			return std::all_of(std::begin(input), std::end(input), 
				[&](const auto & elem) {
					auto & [key, val] = elem;
					return sub_type_l->is(key, heap) && sub_type_r->is(val, heap); 
				}
			);
		}



		Value type_cast_map(const Value & value, Heap & heap, const TypeNode * sub_type_l, const TypeNode * sub_type_r) {
			return type_is_map(value, heap, sub_type_l, sub_type_r) 
				? value
				: value::null; 
		}



		Value type_cast_tuple(const Value & value, Heap & heap, const TypeNode * sub_type_l, const TypeNode * sub_type_r) {
			return type_is_map(value, heap, sub_type_l, sub_type_r) 
				? value
				: value::null; 
		}



		Value type_cast_tuple_n(const Value & value, Heap & heap, const std::vector<const TypeNode *> & sub_types) {
			if(is_array_or_tuple(value)) {
				const Array & input = heap.read<Array>(value);
				if(std::size(input) != std::size(sub_types)) return value::null;
				Array output;
				for(std::size_t i = 0; i < std::size(input); ++i) {
					output.push_back(sub_types[i]->cast(input[i], heap));
				}
				return value::tuple(heap.alloc(std::move(output)));
			}
			else {
				return value::null;
			}
		}



		bool type_is_tuple_n(const Value & value, Heap & heap, const std::vector<const TypeNode *> & sub_types) {
			if(!is_tuple(value)) return false;
			const Array & input = heap.read<Array>(value);
			if(std::size(input) != std::size(sub_types)) return false;
			for(std::size_t i = 0; i < std::size(input); ++i) {
				if(!sub_types[i]->is(input[i], heap)) return false;
			}
			return true;
		}



		template<typename Node>
		std::pair<const TypeNode*, const std::uint8_t*> make_primary_type(VmCore & core, const std::uint8_t * code) {
			auto end = code + 1;
			std::vector<std::uint8_t> bytes {code, end}; 
			if(!core.type_table.contains(bytes)) {
				auto node = std::make_unique<Node>();
				core.type_table[bytes] = std::move(node);
			}
			return { core.type_table.at(bytes).get(), end };
		}



		template<typename Node>
		std::pair<const TypeNode*, const std::uint8_t*> make_unary_type(VmCore & core, const std::uint8_t * code) {
			auto [sub_node, end] = make_type(core, code + 1);
			std::vector<std::uint8_t> bytes {code, end}; 
			if(!core.type_table.contains(bytes)) {
				auto node = std::make_unique<Node>(sub_node);
				core.type_table[bytes] = std::move(node);
			}
			return { core.type_table.at(bytes).get(), end };
		}



		template<typename Node>
		std::pair<const TypeNode*, const std::uint8_t*> make_n_ary_type(VmCore & core, const std::uint8_t * code) {
			auto number = read_uint_64(code + 1);
			std::vector<const TypeNode *> sub_types;
			auto current = code + 1 + 8;
			for(std::size_t i = 0; i < number; ++i) {
				auto [node, next] = make_type(core, current);
				current = next;
				sub_types.push_back(node);
			}
			auto end = current;
			std::vector<std::uint8_t> bytes {code, end}; 
			if(!core.type_table.contains(bytes)) {
				auto node = std::make_unique<Node>(std::move(sub_types));
				core.type_table[bytes] = std::move(node);
			}
			return { core.type_table.at(bytes).get(), end };
		}



		template<typename Node>
		std::pair<const TypeNode*, const std::uint8_t*> make_binary_type(VmCore & core, const std::uint8_t * code) {
			auto [sub_node_l, mid] = make_type(core, code + 1);
			auto [sub_node_r, end] = make_type(core, mid);
			std::vector<std::uint8_t> bytes {code, end}; 
			if(!core.type_table.contains(bytes)) {
				auto node = std::make_unique<Node>(sub_node_l, sub_node_r);
				core.type_table[bytes] = std::move(node);
			}
			return { core.type_table.at(bytes).get(), end };
		}



		template<typename Node>
		std::pair<const TypeNode*, const std::uint8_t*> make_number_type(VmCore & core, const std::uint8_t * code) {
			auto end = code + 1 + 8;
			std::vector<std::uint8_t> bytes {code, end}; 
			auto number = read_uint_64(code + 1);
			if(!core.type_table.contains(bytes)) {
				auto node = std::make_unique<Node>(number);
				core.type_table[bytes] = std::move(node);
			}
			return { core.type_table.at(bytes).get(), end };
		}






		TypeResult make_type_any(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"any", always_true, no_cast>;
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_null(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"null",
				simple_check_for<is_null>,
				type_cast_null
			>;
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_bool(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"bool",
				simple_check_for<is_bool>,
				type_cast_bool
			>; 
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_char(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"char",
				simple_check_for<is_char>,
				type_cast_char
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_int(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"int",
				simple_check_for<is_int>,
				type_cast_int
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_float(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"float",
				simple_check_for<is_float>,
				type_cast_float
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_string(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"string",
				simple_check_for<is_string>,
				type_cast_string
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_array(VmCore & core, const std::uint8_t * code) {
			using Node = UnaryTypeNode<"array", type_is_array, type_cast_array>;    
			return make_unary_type<Node>(core, code);
		}

		TypeResult make_type_tuple(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"tuple",
				simple_check_for<is_tuple>,
				ret_if_or_null<is_tuple>
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_tuple_n(VmCore & core, const std::uint8_t * code) {
			using Node = NAryTypeNode<"tuple", type_is_tuple_n, type_cast_tuple_n>; 
			return make_n_ary_type<Node>(core, code);
		}

		TypeResult make_type_fx(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"fx",
				simple_check_for<is_fxptr>,
				ret_if_or_null<is_fxptr>
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_fx_n(VmCore & core, const std::uint8_t * code) {
			using Node = NumericTypeNode<"fx", type_is_fx_n, type_cast_fx_n>;    
			return make_number_type<Node>(core, code);
		}

		TypeResult make_type_istream(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"istream",
				simple_check_for<is_istream>,
				ret_if_or_null<is_istream>
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_ostream(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"ostream",
				simple_check_for<is_ostream>,
				ret_if_or_null<is_ostream>
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_iter(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"iter",
				simple_check_for<is_iterator>,
				ret_if_or_null<is_iterator>
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_stop(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"stop",
				simple_check_for<is_iterator_stop>,
				ret_if_or_null<is_iterator_stop>
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_rng(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"rng",
				simple_check_for<is_rng>,
				ret_if_or_null<is_rng>
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_clock(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"clock",
				simple_check_for<is_clock>,
				ret_if_or_null<is_clock>
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_type(VmCore & core, const std::uint8_t * code) {
			using Node = PrimaryTypeNode<"type",
				simple_check_for<is_type>,
				ret_if_or_null<is_type>
			>;    
			return make_primary_type<Node>(core, code);
		}

		TypeResult make_type_queue(VmCore & core, const std::uint8_t * code) {
			using Node = UnaryTypeNode<"queue", type_is_queue, type_cast_queue>;    
			return make_unary_type<Node>(core, code);
		}

		TypeResult make_type_stack(VmCore & core, const std::uint8_t * code) {
			using Node = UnaryTypeNode<"stack", type_is_stack, type_cast_stack>;    
			return make_unary_type<Node>(core, code);
		}

		TypeResult make_type_map(VmCore & core, const std::uint8_t * code) {
			using Node = BinaryTypeNode<"map", type_is_map, type_cast_map>;    
			return make_binary_type<Node>(core, code);
		}
	}



	std::pair<const TypeNode*, const std::uint8_t*> make_type(VmCore & core, const std::uint8_t * code) {
		switch (*code) {
			case type_code::ANY: return make_type_any(core, code);
			case type_code::NVLL: return make_type_null(core, code);
			case type_code::BOOL: return make_type_bool(core, code);
			case type_code::CHAR: return make_type_char(core, code);
			case type_code::INT: return make_type_int(core, code);
			case type_code::FLOAT: return make_type_float(core, code);
			case type_code::STRING: return make_type_string(core, code);
			case type_code::ARRAY: return make_type_array(core, code);
			case type_code::TUPLE: return make_type_tuple(core, code);
			case type_code::TUPLE_N: return make_type_tuple_n(core, code);
			case type_code::FX: return make_type_fx(core, code);
			case type_code::FX_N: return make_type_fx_n(core, code);
			case type_code::ISTREAM: return make_type_istream(core, code);
			case type_code::OSTREAM: return make_type_ostream(core, code);
			case type_code::ITERATOR: return make_type_iter(core, code);
			case type_code::ITERATOR_STOP: return make_type_stop(core, code);
			case type_code::RNG: return make_type_rng(core, code);
			case type_code::CLOCK: return make_type_clock(core, code);
			case type_code::TYPE: return make_type_type(core, code);
			case type_code::QUEUE: return make_type_queue(core, code);
			case type_code::STACK: return make_type_stack(core, code);
			case type_code::MAP: return make_type_map(core, code);
			default: throw std::runtime_error{""};
		}
	}
}