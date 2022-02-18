#include "LtnVM.hxx"
#include "TypeCheck.hxx"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace ltn::vm {

	void pushAll(auto & array, Register & reg, std::uint64_t size) {
		if(!size) return;
		const auto value = reg.pop();
		pushAll(array, reg, size -1);
		array.push_back(value);
	} 

	void LtnVM::newarr(){
		const auto ptr = this->heap.alloc<Array>({});
		auto & arr = this->heap.read<Array>(ptr).get(); 
		const auto size = this->fetchUint();
		pushAll(arr, this->reg, size);
		this->reg.push({ ptr, Value::Type::ARRAY });
		this->heap.collectGarbage(this->stack, this->reg);
	}


	void LtnVM::newstr() {
		const auto size = this->fetchUint();
		const auto cstr = this->fetchStr();
		std::string str(cstr, cstr + size); 
		const auto ptr = this->heap.alloc<String>({std::move(str)});
		this->reg.push({ ptr, Value::Type::STRING });
		this->pc += size;
		this->heap.collectGarbage(this->stack, this->reg);
	}


	void LtnVM::newout() {
		const auto addOut = [&] (auto && out) {
			using T = decltype(out)&&;
			const auto ptr = this->heap.alloc<OStream>(std::forward<T>(out));
			this->reg.push({ ptr, Value::Type::OSTREAM });
			this->heap.collectGarbage(this->stack, this->reg);
		};

		const auto variant = this->fetchByte();
		switch (variant) {
			case 0: return addOut(OStream{std::cout});
			case 1: {
				const auto ref = this->reg.pop();
				if(!isStr(ref)) {
					throw except::invalidArgument();
				}
				const auto & path = this->heap.read<String>(ref.u).str;
				return addOut(OStream{std::make_unique<std::ofstream>(path)});
			}
		}
		throw std::runtime_error{"Unknow output variant"};
	}


	void LtnVM::newin() {
		const auto addIn = [&] (auto && in) {
			using T = decltype(in)&&;
			const auto ptr = this->heap.alloc<IStream>(std::forward<T>(in));
			this->reg.push({ ptr, Value::Type::ISTREAM });
			this->heap.collectGarbage(this->stack, this->reg);
		};

		const auto variant = this->fetchByte();
		switch (variant) {
			case 0: return addIn(IStream{std::cin});
			case 1: {
				const auto ref = this->reg.pop();
				if(!isStr(ref)) {
					throw except::invalidArgument();
				}
				const auto & path = this->heap.read<String>(ref.u).str;
				if(!std::filesystem::exists(path)) {
					throw except::cannotOpenFile(path);
				}
				return addIn(IStream{std::make_unique<std::ifstream>(path)});
			}
		}
		throw std::runtime_error {"Unknow input variant"};
	}


	void LtnVM::newclock() {
		const auto ptr = this->heap.alloc<Clock>({});
		this->reg.push({ ptr, Value::Type::CLOCK });
		this->heap.collectGarbage(this->stack, this->reg);
	}


	void LtnVM::newstruct() {
		const auto ptr = this->heap.alloc<Struct>({});
		this->reg.push({ ptr, Value::Type::STRUCT });
		this->heap.collectGarbage(this->stack, this->reg);
	}


	void LtnVM::newrange() {
		const auto end = this->reg.pop();
		const auto begin = this->reg.pop();
		const auto array = this->reg.pop();
		
		if(!isArr(array)) throw except::invalidArgument();
		if(!isInt(begin)) throw except::invalidArgument();
		if(!isInt(end))   throw except::invalidArgument();
		
		const auto range = this->heap.alloc<Range>({array.u, begin.i, end.i});
		this->reg.push(Value{range, Value::Type::RANGE});
	}


	void LtnVM::newstack() {
		const auto ref = this->heap.alloc<Deque>({});
		this->reg.push({ ref, Value::Type::STACK });
		this->heap.collectGarbage(this->stack, this->reg);
	}


	void LtnVM::newqueue() {
		const auto ref = this->heap.alloc<Deque>({});
		this->reg.push({ ref, Value::Type::QUEUE });
		this->heap.collectGarbage(this->stack, this->reg);
	}


	void LtnVM::newmap() {
		const auto ref = this->heap.alloc<Map>(Map{this->heap});
		this->reg.push({ ref, Value::Type::MAP });
		this->heap.collectGarbage(this->stack, this->reg);
	}


	void LtnVM::newfx(){
		const auto address = this->fetchUint(); 
		const auto params = this->fetchUint();
		const auto ref = this->heap.alloc<FxPointer>({address, params, {}});
		this->reg.push(Value{ref, Value::Type::FX_PTR});
	}
}