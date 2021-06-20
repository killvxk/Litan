#include "Ltnc.hxx"
#include <iostream>
#include "LtncLexer.hxx"
#include "LtncParser.hxx"
#include "LtncCompiler.hxx"
#include "LtnFileIO.hxx"
std::string ltnc::LTNC::compile(
	const std::string & source,
	const CompilerSettings & settings,
	bool silent) const {

	ltnc::Lexer lexer;
	ltnc::Parser parser;
	ltnc::Compiler compiler;



	std::string code = source 
		+ ltn::readFile("stdlib/stdbasics.ltn") 
		+ ltn::readFile("stdlib/stdmath.ltn") 
		+ ltn::readFile("stdlib/stdarr.ltn") 
		+ ltn::readFile("stdlib/stdio.ltn") 
		+ ltn::readFile("stdlib/stdexept.ltn")
		+ ltn::readFile("stdlib/stdstring.ltn");

	if(!silent) std::cout << ">> Tokenization..." << std::endl;
	auto tokens = lexer.tokenize(code);
	if(!silent) std::cout << ">> Parsing..." << std::endl;
	auto ast = parser.parse(tokens);
	if(!silent) std::cout << ">> Compiling..." << std::endl;
	auto asmb = compiler.compile(ast, settings);
	if(!silent) std::cout << ">> Done compiling code!" << std::endl;
	std::cout << std::endl;
	return asmb;

}