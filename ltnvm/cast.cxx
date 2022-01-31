#include "cast.hxx"
#include "TypeCheck.hxx"

namespace ltn::vm::cast {
	namespace {
		template<typename To>
		To parseValue(const std::string & str) {
			To to;
			std::stringstream ss{str};
			ss >> to;
			if(ss.fail()) {
				throw std::runtime_error{"Cannot parse string to value"};
			}
			return to;
		}
	}


	std::string to_string(Value value, Heap & heap) {
		if(isNull(value)) {
			return "null";
		}
		if(isBool(value)) {
			std::stringstream ss;
			ss << std::boolalpha << value.b;
			return ss.str();
		}
		if(isInt(value)) {
			std::stringstream ss;
			ss << value.i;
			return ss.str();
		}
		if(isFloat(value)) {
			std::stringstream ss;
			ss << value.f;
			return ss.str();
		}
		if(isStr(value)) {
			return heap.read<String>(value.u).str;
		}
		if(isArr(value)) {
			const auto & arr = heap.read<Array>(value.u);
			std::stringstream ss;
			ss << "[";
			for(std::size_t i = 0; i < arr.arr.size(); i++) {
				ss << ((i) ? (", ") : (""));
				ss << to_string(arr.arr[i], heap);
			}
			ss << "]";
			return ss.str();
		}
		if(isOStream(value)) {
			return "<ostream>";
		}
		if(isIStream(value)) {
			return "<istream>";
		}
		if(isFxPtr(value)) {
			const auto & fxPtr = heap.read<FxPointer>(value.u);
			std::stringstream ss;
			ss << "<fx:";
			ss << fxPtr.address << "," << fxPtr.params << ">";
			return  ss.str();
		}
		if(isClock(value)) {
			const auto & clock = heap.read<Clock>(value.u);
			std::stringstream ss;
			ss << "<clock: " << clock.getSeconds() << "s>";
			return ss.str();
		}
		throw std::runtime_error{"Cannot convert to string"};
	}



	std::int64_t to_int(Value value, Heap & heap) {
		if(isInt(value)) {
			return value.i;
		}
		if(isFloat(value)) {
			return static_cast<std::int64_t>(value.f);
		}
		if(isStr(value)) {
			const auto & str = heap.read<String>(value.u);
			return parseValue<std::int64_t>(str.str); 
		}
		throw std::runtime_error{"Cannot convert to integer"};
	}



	double to_float(Value value, Heap & heap) {
		if(isInt(value)) {
			return static_cast<double>(value.i);
		}
		
		if(isFloat(value)) {
			return value.f;
		}

		if(isStr(value)) {
			const auto & str = heap.read<String>(value.u);
			return parseValue<double>(str.str); 
		}
		
		if (isClock(value)) {
			const auto & clock = heap.read<Clock>(value.u);
			return clock.getSeconds();
		}
		throw std::runtime_error{"Cannot convert to float"};
	}


	bool to_bool(Value value, Heap & heap) {
		if(isBool(value)) {
			return value.b;
		}

		if(isInt(value)) {
			return value.i;
		}
		
		if(isFloat(value)) {
			return value.f == 0.0;
		}

		if(isNull(value)) {
			return false;
		}
		throw std::runtime_error{"Cannot convert to bool"};
	}
}