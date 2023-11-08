#include "parse.hxx"
#include "ltnc/CompilerError.hxx"

namespace ltn::c {
	namespace {
		using TT = Token::Type;
		CompilerError unexpected_else_condition(const Tokens & tokens) {
			return CompilerError{
				"else must not have a condition",
				location(tokens)
			};
		}
	}



	ast::stmt_ptr parse_else_branch(Tokens & tokens) {
		if(match(TT::ELSE, tokens)) {
			// CHANGE LATER
			if(match(TT::PAREN_L, tokens)) {
				throw unexpected_else_condition(tokens);
			}
			return parse_statement(tokens);
		}
		return std::make_unique<ast::NoOp>(location(tokens));
	}



	ast::stmt_ptr parse_if_else(Tokens & tokens) {
		if(match(TT::IF, tokens)) {
			auto expr = parse_condition(tokens); 
			auto ifBody = parse_statement(tokens); 
			auto elseBody = parse_else_branch(tokens); 
			return std::make_unique<ast::IfElse>(
				std::move(expr),
				std::move(ifBody),
				std::move(elseBody),
				location(tokens));
		}
		return nullptr;
	}
}