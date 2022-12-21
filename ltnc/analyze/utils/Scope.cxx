#include "Scope.hxx"
#include "ltnc/CompilerError.hxx"

namespace ltn::c {
	void Scope::set_return(const std::string & return_point) {
		this->return_point = return_point;
	}

	// defines new variable and prevents disambiguations
	// Variables in outer scope can not be hidden
	Variable Scope::insert(
		const std::string & name,
		const SourceLocation & location,
		const type::Type & type) {
		if(this->vars.contains(name)) {
			throw CompilerError{"Redefintion of variable " + name, location};
		}
		const auto addr = size();
		const Variable var{
			.address = addr,
			.type = type,
		}; 
		this->vars.insert({name, var});
		return var;
	}


	void Scope::add_type(const std::string & name, const type::Type & type) {
		this->type_map.insert({name, type});
	}



	void Scope::inherit_types_from(const Scope & scope) {
		for(const auto & [key, val] : scope.type_map) {
			this->type_map.insert({key, val});
		}
	}
}
