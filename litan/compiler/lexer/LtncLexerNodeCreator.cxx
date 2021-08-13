#include "LtncLexerNodeCreator.hxx"
#include <map>

ltn::c::LexerNode ltn::c::LexerNodeCreator::chr(char chr, std::vector<LexerNode> nodes) {
	return LexerNode([chr, nodes](LexerPackage & lexPkg) {
		if(lexPkg.match(chr))  {
			for(const LexerNode & node : nodes) {
				if(node.eval(lexPkg)) {
					return true;
				}
			}
		}
		return false;
	});
}


ltn::c::LexerNode ltn::c::LexerNodeCreator::chr(char chr, TokenType type) {
	return LexerNode([chr, type](LexerPackage & lexPkg) {
		if(lexPkg.match(chr))  {
			lexPkg.newToken(type);
			return true;
		}
		return false;
	});
}


ltn::c::LexerNode ltn::c::LexerNodeCreator::chr(char chr, TokenType type, std::vector<LexerNode> nodes) {
	return LexerNode([chr, nodes, type](LexerPackage & lexPkg) {
		if(lexPkg.match(chr))  {
			for(const LexerNode & node : nodes) {
				if(node.eval(lexPkg)) {
					return true;
				}
			}
			lexPkg.newToken(type);
			return true;
		}
		return false;
	});
}


ltn::c::LexerNode ltn::c::LexerNodeCreator::connector(std::vector<LexerNode> nodes) {
	return LexerNode([nodes](LexerPackage & lexPkg) {
		for(const LexerNode & node : nodes) {
			if(node.eval(lexPkg)) {
				return true;
			}
		}
		return false;
	});
}


ltn::c::LexerNode ltn::c::LexerNodeCreator::ignoreLine() {
	return LexerNode([](LexerPackage & lexPkg) {
		while (!lexPkg.isAtEnd()){
			if(lexPkg.match('\n')) {
				lexPkg.newLine();
				break;
			} 
			lexPkg.next();
		}
		return true;
	});
}


ltn::c::LexerNode ltn::c::LexerNodeCreator::newLine() {
	return LexerNode([](LexerPackage & lexPkg) {
		if(lexPkg.match('\n'))  {
			lexPkg.newLine();
			return true;
		}
		return false;
	});
}


ltn::c::LexerNode ltn::c::LexerNodeCreator::ignore(char chr) {
	return LexerNode([chr](LexerPackage & lexPkg) {
		if(lexPkg.match(chr)) {
			return true;
		}
		return false;
	});
}


ltn::c::LexerNode ltn::c::LexerNodeCreator::numliteral() {
	return LexerNode([](LexerPackage & lexPkg) {
		if(lexPkg.matchDigit()) {
			bool isFloat = false;
			while(!lexPkg.isAtEnd() && lexPkg.matchDigit());
			if(lexPkg.match('.')) {
				isFloat = true;
				while(!lexPkg.isAtEnd() && lexPkg.matchDigit());
			}
			std::string lexeme = lexPkg.makeLexeme();
			TokenType type = isFloat ? TokenType::FLOAT_LITERAL : TokenType::INT_LITERAL;
			lexPkg.newToken(type);
			return true;
		}
		return false;
	});
}


ltn::c::LexerNode ltn::c::LexerNodeCreator::identifier() {

	return LexerNode([](LexerPackage & lexPkg) {
		const static std::map<std::string, TokenType> keywords = {
			{"typedef", TokenType::TYPEDEF},
			{"var", TokenType::VAR},
			{"copy", TokenType::COPY},
			{"nullptr", TokenType::NUL},
			{"namespace", TokenType::NAMESPACE},
			{"function", TokenType::FX},
			{"if", TokenType::IF},
			{"else", TokenType::ELSE},
			{"for", TokenType::FOR},
			{"while", TokenType::WHILE},
			{"return", TokenType::RETURN},
			{"repeat", TokenType::REPEAT},
			{"struct", TokenType::STRUCT},
			{"template", TokenType::TEMPLATE},
			{"asm", TokenType::ASM},
			{"true", TokenType::TRUE},
			{"false", TokenType::FALSE}
		};
		if(lexPkg.matchAlpha()) {
			while(lexPkg.matchAlpha() || lexPkg.match('_') || lexPkg.matchDigit());
			std::string lexeme = lexPkg.makeLexeme();
			if(keywords.contains(lexeme)) {
				lexPkg.newToken(keywords.at(lexeme));
			}
			else {
				lexPkg.newToken(TokenType::IDENTIFIER);
			}
			return true;
		}
		return false;
	});
}


ltn::c::LexerNode ltn::c::LexerNodeCreator::string() {
	return LexerNode([](LexerPackage & lexPkg) {
		if(lexPkg.match('"')){
			while(!lexPkg.match('"')) {
				if(lexPkg.match('\\')) {
					bool valid = 
						lexPkg.match('n') ||
						lexPkg.match('\\') ||
						lexPkg.match('t') ||
						lexPkg.match('"');
					if(!valid) {
						lexPkg.error("Invalid escape sequence");
					}
				}
				lexPkg.next();
				if(lexPkg.isAtEnd()) {
					lexPkg.error("Unterminated string");
				}
			}
			std::string lexeme = lexPkg.makeLexeme(); 
			lexPkg.newToken(
				TokenType::STRING_LITERAL,
				lexeme.substr(1, lexeme.size()-2));
			return true;
		}
		return false;
	});
}
