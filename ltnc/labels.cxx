#include "labels.hxx"
#include <sstream>
#include "ltnc/type/to_string.hxx"

namespace ltn::c {
	namespace {
		std::uint64_t unique_number() {
			static std::uint64_t counter = 0; 
			return counter++;
		}



		Label internal(const std::string & name) {
			return Label { Label::UseCase::INTERNAL, name };
		}



		Label external(const std::string & name) {
			return Label { Label::UseCase::EXTERNAL, name };
		}
	}



	Label make_lambda_label() {
		std::ostringstream oss;
		oss << "LAMBDA" << unique_number();
		return internal(oss.str());
	}



	Label make_function_label(
		const Namespace & namespaze,
		const std::string & name,
		std::uint64_t arity) {

		std::ostringstream oss;
		oss << namespaze.to_string() << name << "(" << arity << ")";
		return external(oss.str());
	}



	Label make_function_label(const ast::Functional & fx) {
		return make_function_label(
			fx.get_resolve_namespace(),
			fx.get_resolve_name(),
			fx.parameters.size()
		);
	}



	Label make_template_label(
		const ast::FunctionTemplate & tmpl,
		const std::vector<type::Type> & args) {
		const auto label = make_function_label(*tmpl.fx);
		return derive_template(label, args);
	}



	Label derive_skip(const Label & label) {
		return internal(label.get_name() + "_SKIP");
	}



	Label derive_except(const Label & label) {
		return internal(label.get_name() + "_EXCEPT");
	}


	Label derive_template(const Label & label, const std::vector<type::Type> & args) {
		std::ostringstream oss;
		oss << label.get_name();
		for(const auto & arg : args) {
			oss << "_" << type::to_string(arg);
		}
		return internal(oss.str());
	}
}