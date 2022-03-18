#include "parse.hxx"

namespace ltn::c::parse {
	namespace {
		using TT = ltn::c::lex::Token::Type;
	
		std::string name(lex::Lexer & lexer) {
			if(auto t = lexer.match(TT::INDENTIFIER)) {
				return t->str;
			}
			throw CompilerError{"Expected definition name", lexer.location()};
		}


		std::string equal_sign(lex::Lexer & lexer) {
			if(auto t = lexer.match(TT::ASSIGN)) {
				return t->str;
			}
			throw CompilerError{"Expected =", lexer.location()};
		}
	}


	ast::glob_ptr definition(lex::Lexer & lexer, const ast::Namespace & namespaze) {
		if(lexer.match(TT::DEFINE)) {
			auto global = std::make_unique<ast::Global>(lexer.location());
			global->name = parse::name(lexer);
			global->namespaze = namespaze;
			equal_sign(lexer);
			global->expr = parse::expression(lexer);
			semicolon(lexer);
			return global;
		}
		return nullptr;
	}
}