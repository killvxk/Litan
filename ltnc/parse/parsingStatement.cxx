#include "parsing.hxx"
#include "ltnc/lex/lexing.hxx"
#include "ltnc/CompilerError.hxx"

namespace ltn::c::parse {
	namespace {
		using TT = ltn::c::lex::Token::Type;
	}

	std::unique_ptr<ast::StatementExpression> expr(lex::Lexer & lexer) {
		// Expression
		auto expr = expression(lexer);
		if(auto semicolon = lexer.match(TT::SEMICOLON)) {
			return std::make_unique<ast::StatementExpression>(std::move(expr));
		}
		throw CompilerError{"missing semicolon", lexer.inLine()};
	}

	std::unique_ptr<ast::Statement> statement(lex::Lexer & lexer) {
		if(auto stmt = block(lexer)) return stmt;
		if(auto stmt = ifElse(lexer)) return stmt;
		if(auto stmt = whileLoop(lexer)) return stmt;
		return expr(lexer);
	}
}