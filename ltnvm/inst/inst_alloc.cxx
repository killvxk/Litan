#include "instructions.hxx"
#include "ltnvm/type_check.hxx"
#include "ltnvm/convert.hxx"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace ltn::vm::inst {

	void pushAll(auto & array, Stack & stack, std::uint64_t size) {
		if(!size) return;
		const auto value = stack.pop();
		pushAll(array, stack, size -1);
		array.push_back(value);
	} 



	void newarr(VmCore & core){
		const auto ptr = core.heap.alloc<Array>({});
		auto & arr = core.heap.read<Array>(ptr); 
		const auto size = core.fetch_uint();
		pushAll(arr, core.stack, size);
		core.stack.push({ ptr, Value::Type::ARRAY });
		core.heap.collect_garbage(core.stack, core.static_variables);
	}



	void newstr(VmCore & core) {
		const auto size = core.fetch_uint();
		const auto cstr = core.fetch_str();
		std::string str(cstr, cstr + size); 
		const auto ptr = core.heap.alloc<String>({std::move(str)});
		core.stack.push({ ptr, Value::Type::STRING });
		core.pc += size;
		core.heap.collect_garbage(core.stack, core.static_variables);
	}



	void newout(VmCore & core) {
		const auto add_out = [&] (auto && out) {
			using T = decltype(out)&&;
			const auto ptr = core.heap.alloc<OStream>(std::forward<T>(out));
			core.stack.push({ ptr, Value::Type::OSTREAM });
			core.heap.collect_garbage(core.stack, core.static_variables);
		};

		const auto variant = core.fetch_byte();
		switch (variant) {
			case 0: return add_out(OStream{std::cout});
			case 1: {
				const auto openmode = convert::to_int(core.stack.pop());
				const auto ref = core.stack.pop();
				if(!is_string(ref)) {
					throw except::invalid_argument();
				}
				const auto & path = core.heap.read<String>(ref.u);

				const auto flags =
					openmode ? std::ios::app : std::ios::trunc;

				auto fout = std::make_unique<std::ofstream>(path, flags);
				return add_out(OStream{std::move(fout)});
			}
			case 2: {
				return add_out(OStream{std::make_unique<std::ostringstream>()});
			}
		}
		throw std::runtime_error{"Unknow output variant"};
	}



	void newin(VmCore & core) {
		const auto add_in = [&] (auto && in) {
			using T = decltype(in)&&;
			const auto ptr = core.heap.alloc<IStream>(std::forward<T>(in));
			core.stack.push({ ptr, Value::Type::ISTREAM });
			core.heap.collect_garbage(core.stack, core.static_variables);
		};

		const auto variant = core.fetch_byte();
		switch (variant) {
			case 0: return add_in(IStream{std::cin});
			case 1: {
				const auto ref = core.stack.pop();
				if(!is_string(ref)) {
					throw except::invalid_argument();
				}
				const auto & path = core.heap.read<String>(ref.u);
				if(!std::filesystem::exists(path)) {
					throw except::cannot_open_file(path);
				}
				return add_in(IStream{std::make_unique<std::ifstream>(path)});
			}
			case 2: {
				const auto ref = core.stack.pop();
				if(!is_string(ref)) {
					throw except::invalid_argument();
				}
				const auto & str = core.heap.read<String>(ref.u);
				return add_in(IStream{std::make_unique<std::istringstream>(str)});
			}
		}
		throw std::runtime_error {"Unknow input variant"};
	}



	void newclock(VmCore & core) {
		const auto ptr = core.heap.alloc<Clock>({});
		core.stack.push({ ptr, Value::Type::CLOCK });
		core.heap.collect_garbage(core.stack, core.static_variables);
	}



	void newstruct(VmCore & core) {
		const auto ptr = core.heap.alloc<Struct>({});
		core.stack.push({ ptr, Value::Type::STRUCT });
		core.heap.collect_garbage(core.stack, core.static_variables);
	}



	void newstack(VmCore & core) {
		const auto ref = core.heap.alloc<Deque>({});
		core.stack.push({ ref, Value::Type::STACK });
		core.heap.collect_garbage(core.stack, core.static_variables);
	}



	void newqueue(VmCore & core) {
		const auto ref = core.heap.alloc<Deque>({});
		core.stack.push({ ref, Value::Type::QUEUE });
		core.heap.collect_garbage(core.stack, core.static_variables);
	}



	void newmap(VmCore & core) {
		const auto ref = core.heap.alloc<Map>(Map{Comparator{&core.heap}});
		core.stack.push({ ref, Value::Type::MAP });
		core.heap.collect_garbage(core.stack, core.static_variables);
	}


	void newfx(VmCore & core){
		const auto address = core.fetch_uint(); 
		const auto params = core.fetch_uint();
		const auto ref = core.heap.alloc<FxPointer>({address, params, {}});
		core.stack.push(Value{ref, Value::Type::FX_PTR});
	}



	void newrng(VmCore & core){
		const auto type = core.fetch_byte();
		switch (type) {
		case 0x00: { 
			const auto seed = std::random_device{}();
			auto rng = RandomEngine{std::mt19937_64{seed}};
			const auto ref = core.heap.alloc<RandomEngine>(std::move(rng));
			core.stack.push(value::rng(ref));
			return;
		}

		case 0x01: { 
			const auto signed_seed = convert::to_int(core.stack.pop());
			const auto seed = static_cast<std::uint64_t>(signed_seed);
			auto rng = RandomEngine{std::mt19937_64{seed}};
			const auto ref = core.heap.alloc<RandomEngine>(std::move(rng));
			core.stack.push(value::rng(ref));
			return;
		}
		
		default: throw std::runtime_error{"Unknown RandonEngine type"};
		}
	}
}