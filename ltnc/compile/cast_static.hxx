#pragma once
#include "ltnc/SourceLocation.hxx"
#include "InstructionBuffer.hxx"
#include "ltnc/type/Type.hxx"

namespace ltn::c {
	InstructionBuffer cast_static(	
		const type::Type & from,
		const type::Type & to,
		const SourceLocation & location);
}