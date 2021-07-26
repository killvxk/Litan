#pragma once
#include <string>
#include "LtncVar.hxx"
#include "LtncType.hxx"
#include "LtncFunction.hxx"
namespace ltnc {
	class TypeSearch {
	public:
		TypeSearch(const TypeId & typeId);
		const Type * operator()(const Type & type) const;
		const Type * operator()(const Func & func) const;
		const Type * operator()(const Var & var) const;
	private:
		TypeId typeId;
	};
}