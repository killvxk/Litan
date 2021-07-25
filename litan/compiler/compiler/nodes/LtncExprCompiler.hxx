#pragma once
#include <memory>
#include "LtncType.hxx"
#include "LtncExpr.hxx"
#include "LtncScope.hxx"
#include "LtncCompilerPack.hxx"
#include "LtncExprInfo.hxx"
#include "LtncEvaluator.hxx"
namespace ltnc {
	// Creates a variaty of expressions
	class VariCompiler;
	class ExprCompiler {
	public:
		ExprCompiler(
			const VariCompiler & variCompiler);
		ExprInfo compile(CompilerPack & compPkg, const std::shared_ptr<Expr> & expr) const;

		// compiles generic expression
		ExprInfo compileExpr(CompilerPack & compPkg, std::shared_ptr<Expr> expr) const;
		
		// compiles generic binary expression
		ExprInfo compileBinary(CompilerPack & compPkg, std::shared_ptr<ExprBinary> expr) const;


		// optimzed binary operator
		ExprInfo buildBinary(
			CompilerPack & compPkg,
			std::shared_ptr<ExprBinary> expr,
			const std::string & command,
			const Evaluator * eval = nullptr) const;

		ExprInfo compileUnary(CompilerPack & compPkg, std::shared_ptr<ExprUnary> expr) const;

		// int literal
		ExprInfo compileIntLit(CompilerPack & compPkg, std::shared_ptr<ExprIntLiteral> expr) const;
		// float literal
		ExprInfo compileFltLit(CompilerPack & compPkg, std::shared_ptr<ExprFltLiteral> expr) const;
		// string literal
		ExprInfo compileStrLit(CompilerPack & compPkg, std::shared_ptr<ExprStrLiteral> expr) const;
		
		// function call
		ExprInfo compileCall(CompilerPack & compPkg, std::shared_ptr<ExprCall> expr) const;

	private:
		// returns i for int and f for float
		// throws if types do not match or type is not and int or flt
		std::string getSuffux(const ExprInfo & l, const ExprInfo & r) const;

		const VariCompiler & variCompiler;


	};
}