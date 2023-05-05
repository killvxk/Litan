#include "parse.hxx"
#include "ltnc/CompilerError.hxx"
#include "stdxx/iife.hxx"
#include <sstream>
#include <bitset>
namespace ltn::c {
	namespace {
		using TT = Token::Type;



		CompilerError expected(std::string token, const SourceLocation & location) {
			return {"Expected " + token, location};
		}



		template<class TempType, TT tt, auto base>
		ast::expr_ptr parse_integer(Tokens & tokens) {
			if(auto token = match(tt, tokens)) {
				std::stringstream iss{token->str};
				TempType value;
				iss >> base >> value;
				return std::make_unique<ast::Integer>(value, location(tokens)); 
			}
			return nullptr;
		}
		constexpr auto parse_integer_dec = parse_integer<
			std::int64_t,
			TT::INTEGER,
			std::dec>;
		constexpr auto parse_integer_hex = parse_integer<
			std::int64_t,
			TT::INTEGER_HEX,
			std::hex>;
		constexpr auto parse_integer_bin = parse_integer<
			std::bitset<64>,
			TT::INTEGER_BIN,
			std::dec>;



		ast::expr_ptr parse_character(Tokens & tokens) {
			if(auto t = match(TT::CHAR, tokens)) {
				const auto chr = static_cast<std::uint8_t>(t->str.front());
				return std::make_unique<ast::Char>(chr, location(tokens)); 
			}
			return nullptr;
		}



		ast::expr_ptr parse_null(Tokens & tokens) {
			if(auto t = match(TT::NVLL, tokens)) {
				return std::make_unique<ast::Null>(location(tokens)); 
			}
			return nullptr;
		}



		ast::expr_ptr parse_floating(Tokens & tokens) {
			if(auto token = match(TT::FLOAT, tokens)) {
				std::istringstream iss{token->str};
				stx::float64_t value;
				iss >> value;
				return std::make_unique<ast::Float>(value, location(tokens)); 
			}
			return nullptr;
		}



		ast::expr_ptr parse_boolean(Tokens & tokens) {
			if(auto token = match(TT::TRUE, tokens)) {
				return std::make_unique<ast::Bool>(true, location(tokens)); 
			}
			if(auto token = match(TT::FALSE, tokens)) {
				return std::make_unique<ast::Bool>(false, location(tokens)); 
			}
			return nullptr;
		}



		ast::expr_ptr parse_string(Tokens & tokens) {
			if(auto token = match(TT::STRING, tokens)) {
				return std::make_unique<ast::String>(
					token->str,
					location(tokens)); 
			}
			return nullptr;
		}



		auto parse_parameters(
			Tokens & tokens,
			auto fxParam,
			TT close = TT::PAREN_R) {
			
			if(match(close, tokens)) {
				 return;
			}
			while(true) {
				fxParam();
				if(match(close, tokens)) {
					return;
				}
				if(!match(TT::COMMA, tokens)) {
					throw expected(",", location(tokens));
				}
			}
		}



		ast::expr_ptr parse_identifier(Tokens & tokens) {
			const auto [name, namespaze] = parse_symbol(tokens);
			return std::make_unique<ast::Var>(name, namespaze, location(tokens));
		}



		ast::expr_ptr parse_fx_pointer(Tokens & tokens) {
			if(match(TT::AMPERSAND, tokens)) {
				std::string name;
				Namespace namespaze;
				std::tie(name, namespaze) = parse_symbol(tokens);
				if(match(TT::PAREN_L, tokens)) {
					auto placeholders = parse_placeholder(tokens);
					auto fx_ptr = std::make_unique<ast::FxPointer>(
						std::move(name),
						std::move(namespaze),
						std::move(placeholders),
						location(tokens));
					return fx_ptr;
				}
				throw expected("(", location(tokens));
			}
			return nullptr; 
		}



		ast::expr_ptr parse_iife(Tokens & tokens) {
			if(match(TT::IIFE, tokens)) {
				auto body = parse_block(tokens);
				return std::make_unique<ast::Iife>(
					location(tokens),
					std::move(body)
				);
			}
			else return nullptr;
		}
	}



	std::uint64_t parse_placeholder(Tokens & tokens) {
		std::uint64_t placeholders = 0;
		parse_parameters(tokens, [&] {
			if (match(TT::UNDERSCORE, tokens)) {
				++placeholders;
			}
		});
		return placeholders;
	}



	std::pair<std::string, Namespace> parse_symbol(Tokens & tokens) {
		Namespace namespaze;
		if(match(TT::COLONx2, tokens)) {
			namespaze.set_absolute();
		}
		if(const auto & identifier = match(TT::INDENTIFIER, tokens)) {
			namespaze.push_back(identifier->str);
			std::string name = identifier->str;
			while(match(TT::COLONx2, tokens)) {
				if(auto i = match(TT::INDENTIFIER, tokens)) {
					namespaze.push_back(i->str);
					name = i->str;
				}
			}
			namespaze.pop_back();
			return {name, namespaze};
		}
		throw expected("indentifier", location(tokens));
	}



	ast::expr_ptr parse_integral(Tokens & tokens) {
		if(auto expr = parse_integer_dec(tokens)) return expr;
		if(auto expr = parse_integer_bin(tokens)) return expr;
		if(auto expr = parse_integer_hex(tokens)) return expr;
		else return nullptr;
	}



	// parses primary expr
	ast::expr_ptr parse_primary(Tokens & tokens) {
		if(auto expr = parse_integral(tokens)) return expr;
		if(auto expr = parse_character(tokens)) return expr;
		if(auto expr = parse_floating(tokens)) return expr;
		if(auto expr = parse_boolean(tokens)) return expr;
		if(auto expr = parse_null(tokens)) return expr;
		if(auto expr = parse_string(tokens)) return expr;
		if(auto expr = parse_array(tokens)) return expr;
		if(auto expr = parse_parenthesized(tokens)) return expr; 
		if(auto expr = parse_fx_pointer(tokens)) return expr;
		if(auto expr = parse_lambda(tokens)) return expr;
		if(auto expr = parse_iife(tokens)) return expr;
		if(auto expr = parse_expr_switch(tokens)) return expr;
		if(auto expr = parse_reflect(tokens)) return expr;
		if(auto expr = parse_type(tokens)) return expr;
		return parse_identifier(tokens);
	}
}

