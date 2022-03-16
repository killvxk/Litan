#pragma once
#include <sstream>
#include <span>
#include "utils.hxx"
#include "Scope.hxx"
#include "FxTable.hxx"
#include "MemberTable.hxx"
#include "Instructions.hxx"
#include "StmtCode.hxx"
#include "ExprCode.hxx"
#include "ltnc/Config.hxx"
#include "ltnc/CompilerError.hxx"
#include "ltnc/Reporter.hxx"
#include "ltnc/ast/Ast.hxx"

namespace ltn::c::compile {
	struct CompilerInfo {
		const Config & config;
		FxTable & fx_table;
		MemberTable & memberTable;
		std::uint64_t & jump_mark_counter; 
		Reporter & reporter;
	};


	
	// Functional
	std::string functional(const ast::Functional & fx, CompilerInfo & info);
	ExprCode lambda(const ast::Lambda & lm, CompilerInfo & info, Scope & scope);

	// Statement
	StmtCode statement(const ast::Statement & stmt, CompilerInfo & info, Scope & scope);
	StmtCode if_else(const ast::IfElse & stmt, CompilerInfo & info, Scope & scope);
	StmtCode while_loop(const ast::While & stmt, CompilerInfo & info, Scope & scope);
	StmtCode for_loop(const ast::For & stmt, CompilerInfo & info, Scope & scope);
	StmtCode new_variable(const ast::NewVar & stmt, CompilerInfo & info, Scope & scope);
	StmtCode new_const(const ast::NewConst & stmt, CompilerInfo & info, Scope & scope);

	// Expression
	ExprCode expression(const ast::Expression & expr, CompilerInfo & info, Scope & scope);
	ExprCode binary(const ast::Binary & expr, CompilerInfo & info, Scope & scope);
	ExprCode unary(const ast::Unary & expr, CompilerInfo & info, Scope & scope);
	ExprCode primary(const ast::Primary & expr, CompilerInfo & info, Scope & scope);
	ExprCode assignable(const ast::Assignable & expr, CompilerInfo & info, Scope & scope);
	ExprCode addr(const ast::Var & expr, Scope & scope);
	ExprCode read_variable(const ast::Var & expr, CompilerInfo & info, Scope & scope);
	ExprCode read_member_access(const ast::Member & access, CompilerInfo & info, Scope & scope);

	// utils
	std::string make_jump_id(const std::string_view name, CompilerInfo & info);
}	