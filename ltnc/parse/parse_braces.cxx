#include "parse.hxx"
#include "ltnc/CompilerError.hxx"

namespace ltn::c {
	namespace {
		using TT = ltn::c::lex::Token::Type;
	}


	void brace_l(lex::Lexer & lexer) {
		if(!lexer.match(TT::BRACE_L)) {
			throw CompilerError{"Expected {", lexer.location()};
		}
	}



	void brace_r(lex::Lexer & lexer) {
		if(!lexer.match(TT::BRACE_R)) {
			throw CompilerError{"Expected }", lexer.location()};
		}
	}
}