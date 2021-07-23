#include "LtncParserStmt.hxx"


ltnc::ParserStmt::ParserStmt(			
	const ParserNode<StmtBlock> & block,
	const ParserNode<Stmt> & ifelse,
	const ParserNode<Stmt> & assign,
	const ParserNode<Expr> & expr,
	const ParserNode<ExprCall> & call)
:	block(block), ifelse(ifelse), assign(assign), expr(expr), call(call) {}

std::shared_ptr<ltnc::Stmt> ltnc::ParserStmt::eval(ParserPackage & parsePkg) const {
	return this->stmt(parsePkg);
}

std::shared_ptr<ltnc::Stmt> ltnc::ParserStmt::stmt(ParserPackage & parsePkg) const {
	if(auto stmt = this->block.eval(parsePkg)) return stmt;
	if(auto stmt = this->asmBlock(parsePkg)) return stmt;
	
	if(auto stmt = this->assign.eval(parsePkg)) return stmt;
	
	if(auto stmt = this->ifelse.eval(parsePkg)) return stmt;
	
	if(parsePkg.match(TokenType::FOR)) return this->forLoop(parsePkg);
	if(parsePkg.match(TokenType::WHILE)) return this->whileLoop(parsePkg);
	if(parsePkg.match(TokenType::REPEAT)) return this->repeatLoop(parsePkg);

	if(parsePkg.match(TokenType::RETURN)) {
		if (parsePkg.match(TokenType::SEMICOLON)) {
			return std::make_shared<StmtReturn>(nullptr);
		}
		auto expr = this->expr.eval(parsePkg);
		auto stmt = std::make_shared<StmtReturn>(expr);
		if (parsePkg.match(TokenType::SEMICOLON)) {
			return stmt;
		}
		return parsePkg.error("expected ; after return statement");
	}
	auto stmt = std::make_shared<StmtExpr>(this->expr.eval(parsePkg));
	if (parsePkg.match(TokenType::SEMICOLON)) {
		return stmt;
	}
	return parsePkg.error("expected ;");
}



std::shared_ptr<ltnc::StmtFor> ltnc::ParserStmt::forLoop(ParserPackage & parsePkg) const {
	auto loop = std::make_shared<StmtFor>();
	if(parsePkg.match(TokenType::L_PAREN)) {
		if(parsePkg.match(TokenType::IDENTIFIER)) {
			loop->name = parsePkg.prev().string;
			if(parsePkg.match(TokenType::COMMA)) {
				loop->exprFrom = this->expr.eval(parsePkg);
				if(parsePkg.match(TokenType::COMMA)){
					loop->exprTo = this->expr.eval(parsePkg);
					if(parsePkg.match(TokenType::R_PAREN)){
						if(auto block = this->block.eval(parsePkg)){
							loop->stmt = block;
							return loop;
						}
						return parsePkg.error("expected block statement");
					}
					return parsePkg.error("expected )");
				}
				return parsePkg.error("expected ,");
			}
			return parsePkg.error("expected ,");
		}
		return parsePkg.error("expected identfier");
	}
	return parsePkg.error("expected (");
}


std::shared_ptr<ltnc::StmtRepeat> ltnc::ParserStmt::repeatLoop(ParserPackage & parsePkg) const {
	auto loop = std::make_shared<StmtRepeat>();
	if(parsePkg.match(TokenType::L_PAREN)) {
		loop->expr = this->expr.eval(parsePkg);
		if(parsePkg.match(TokenType::R_PAREN)){
			if(auto block = this->block.eval(parsePkg)){
				loop->stmt = block;
				return loop;
			}
			return parsePkg.error("expected block statement");
		}
		return parsePkg.error("expected )");
	}
	return parsePkg.error("expected (");
}


std::shared_ptr<ltnc::StmtWhile> ltnc::ParserStmt::whileLoop(ParserPackage & parsePkg) const {
	auto loop = std::make_shared<StmtWhile>();
	if(parsePkg.match(TokenType::L_PAREN)) {
		loop->expr = this->expr.eval(parsePkg);
		if(parsePkg.match(TokenType::R_PAREN)) {
			if(auto block = this->block.eval(parsePkg)) {
				loop->stmt = block;
				return loop;
			}
			return parsePkg.error("expected {");
		}
		return parsePkg.error("expected )");
	}
	return parsePkg.error("Invalid while loop");
}

std::shared_ptr<ltnc::StmtAsm> ltnc::ParserStmt::asmBlock(ParserPackage & parsePkg) const {
	if(parsePkg.match(TokenType::ASM)){
		auto asmStmt = std::make_shared<StmtAsm>();
		if(parsePkg.match(TokenType::L_BRACE)){
			while(parsePkg.match(TokenType::STRING_LITERAL)) {
				asmStmt->instructions.push_back(parsePkg.prev().string);
			}
			if(parsePkg.match(TokenType::R_BRACE)){
				return asmStmt;
			}
			return parsePkg.error("Expected } after asm block");
		}
		return parsePkg.error("Expected { before asm block and after \"asm\" keyword");
	}
	return nullptr;
}
