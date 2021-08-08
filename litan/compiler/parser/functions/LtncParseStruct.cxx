#include "LtncParserFunctions.hxx"

std::vector<ltnc::StmtVar> parseMembers(ltnc::ParserPackage & parsePkg) {
	std::vector<ltnc::StmtVar> members;
	if(parsePkg.match(ltnc::TokenType::L_BRACE)) {
		while(auto member = ltnc::parse::declareVar(parsePkg)) {
			members.push_back(*member);
		}
		if(parsePkg.match(ltnc::TokenType::R_BRACE)) {
			return members;
		}
		throw ltnc::error::expectedBraceR(parsePkg);
	}
	throw ltnc::error::expectedBraceL(parsePkg);
}

ltnc::TypeId parseTypeId(ltnc::ParserPackage & parsePkg) {
	if(parsePkg.match(ltnc::TokenType::IDENTIFIER)) {
		return ltnc::TypeId(parsePkg.prev().string, parsePkg.ns);
	}
	throw ltnc::error::unnamedStruct(parsePkg);
}

std::shared_ptr<ltnc::DeclStruct> ltnc::parse::declareStruct(ParserPackage & parsePkg) {
	if(parsePkg.match(TokenType::STRUCT)) {
		auto debugInfo = parsePkg.prev().debugInfo; 
		auto typeId = parseTypeId(parsePkg);
		auto members = parseMembers(parsePkg);
		return std::make_shared<DeclStruct>(debugInfo, typeId, members);
	}
	return nullptr;
}