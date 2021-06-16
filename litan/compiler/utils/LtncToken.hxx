#pragma once
#include <string>

namespace ltnc {
	enum class TokenType {
		IDENTIFIER, INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, FNX, CON, VAR, TYPEDEF, INLINE, STRUCT,
		ASSIGN, CALL, RETURN, ASM, NEW, DEL, COPY,
		SEMICOLON, COMMA, DOT,
		PRINT, IF, ELSE, WHILE, FOR, REPEAT,
		L_BRACE, R_BRACE, L_PAREN, R_PAREN,
		PLUS, MINUS, STAR, SLASH, MOD,
		EQUAL, UNEQUAL, SMALLER, BIGGER, SMALLEREQUAL, BIGGEREQUAL,
		LOG_AND, LOG_OR, LOG_XOR, LOG_NOT, 
		BIT_AND, BIT_OR, BIT_XOR, BIT_NOT, 
		___NONE__,
		___EOF___,
	};
	struct Token {
		Token(TokenType type, const std::string & string, unsigned line)
		: type(type), string(string), line(line) {}
		TokenType type;
		std::string string;
		unsigned line;
	};
}