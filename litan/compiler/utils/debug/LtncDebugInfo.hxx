#pragma once
#include <cstdint>
#include <string>
#include <optional>
#include "LtncFunctionSignature.hxx"

namespace ltnc {
	class DebugInfo {
	public:
		DebugInfo(
			unsigned lineNr,
			unsigned column,
			const std::string & lexeme);
		
		DebugInfo();

		DebugInfo withFunction(
			const FunctionSignature & inFunction) const;

		unsigned lineNr;
		unsigned column;
		std::string lexeme;

		std::optional<FunctionSignature> inFunction;
	};
}