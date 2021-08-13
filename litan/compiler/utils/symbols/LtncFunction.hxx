#pragma once
#include "LtncFunctionSignature.hxx"
namespace ltn::c {
	struct Function {
		Function(FunctionSignature signature, std::string jumpMark)
			: signature(signature), jumpMark(jumpMark) {}
		FunctionSignature signature;
		std::string jumpMark;
	};


	inline bool operator==(const Function & fxL, const Function & fxR) {
		return fxL.signature == fxR.signature;
	}

	inline bool operator==(const Function & fx, const FunctionSignature & signature) {
		return fx.signature == signature;
	}
	
	inline bool operator==(const FunctionSignature & signature, const Function & fx) {
		return signature == fx.signature;
	}
}