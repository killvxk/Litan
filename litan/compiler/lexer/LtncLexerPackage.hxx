#pragma once
#include <string>
#include <vector>
#include "LtncToken.hxx"
namespace ltn::c {
	// Data container for Lexer
	// Can be passed through lexing tree
	class LexerPackage {
	public:
		LexerPackage(
			const std::string & source,
			const std::string & sourceName);

		// Returns true if all characters are processed
		bool isAtEnd() const;
		// Matches character and continues
		bool match(char chr);
		// Matches letter and continues
		bool matchAlpha();
		// Matches digit and continues
		bool matchDigit();

		// Increment line counter;
		void newLine();
		// Set start to current
		void sync();
		// Goto to next character
		void next();

		// Returns string between start and current
		std::string makeLexeme() const;
		DebugInfo makeDebugInfo() const;
		DebugInfo makeDebugInfo(const std::string & str) const;
		
		// Create token
		void newToken(TokenType type);
		// Create token with custom string
		void newToken(TokenType type, const std::string & lexeme);
		// Add token
		void addToken(Token token);
		
		const std::vector<Token> getTokens() const;
		// Throws error with extra infos
		std::nullptr_t error(const std::string & msg);

	private:
		unsigned start = 0;
		unsigned current = 0;
		unsigned line = 0;
		unsigned column = 0;
		std::string source;
		std::string sourceName;
		std::vector<Token> tokens;
	};
}