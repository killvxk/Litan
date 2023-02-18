#pragma once
#include <vector>
#include <utility>
#include <any>
#include <bitset>
#include "ltn/casts.hxx"
#include "ltn/Visitor.hxx"
#include "ltnc/Namespace.hxx"
#include "ltnc/Operations.hxx"
#include "ltnc/ast/Node.hxx"


namespace ltn::c::ast {
	class Statement;
	struct Functional;
	struct Function;

	struct Binary;
	struct Unary;
	struct Integer;
	struct Float;
	struct Bool;
	struct Char;
	struct Null;
	struct String;
	struct Array;
	struct Tuple;
	struct Call;
	struct Var;
	struct Index;
	struct Lambda;
	struct FxPointer;
	struct Member;
	struct GlobalVar;
	struct Iife;
	struct Ternary;
	struct Choose;
	struct TypedUnary;
	struct Reflect;
	struct ForwardDynamicCall;
	struct InitStruct;


	using ExprVisitor = Visitor<
		Binary,
		Unary,
		Integer,
		Float,
		Bool,
		Char,
		Null,
		String,
		Array,
		Tuple,
		Call,
		Var,
		Index,
		Lambda,
		FxPointer,
		Member,
		GlobalVar,
		Iife,
		Ternary,
		Choose,
		TypedUnary,
		Reflect,
		ForwardDynamicCall,
		InitStruct
	>;



	struct Expression : public Node {
		Expression(const SourceLocation & location) : Node(location) {}
		
		virtual ~Expression() = default;
		virtual void accept(const ExprVisitor & visitor) const = 0;
	};



	struct ForwardDynamicCall final : public Expression {
		ForwardDynamicCall(
			std::uint64_t address,
			std::uint64_t arity,
			const SourceLocation & location)
			: Expression(location)
			, address{address}
			, arity{arity} {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::uint64_t address;
		std::uint64_t arity;
	};
	


	struct Ternary final : public Expression {
		Ternary(
			const SourceLocation & location,
			std::unique_ptr<Expression> condition,
			std::unique_ptr<Expression> if_branch,
			std::unique_ptr<Expression> else_branch) 
			: Expression(location)
			, condition(std::move(condition))
			, if_branch(std::move(if_branch))
			, else_branch(std::move(else_branch)) {}
	
		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::unique_ptr<Expression> condition;
		std::unique_ptr<Expression> if_branch;
		std::unique_ptr<Expression> else_branch;
		
	};




	struct Unary final : public Expression {
		using Op = UnaryOp;
		
		Unary(
			Op op,
			std::unique_ptr<Expression> expr,
			const SourceLocation & location)
			: Expression(location)
			, op(op)
			, expr(std::move(expr)) {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		Op op;
		std::unique_ptr<Expression> expr;
	};



	struct Binary final : public Expression {
		using Op = BinaryOp;

		Binary(
			Op op,
			std::unique_ptr<Expression> l,
			std::unique_ptr<Expression> r,
			const SourceLocation & location)
			: Expression(location)
			, op(op)
			, l(std::move(l))
			, r(std::move(r)) {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		Op op;
		std::unique_ptr<Expression> l;
		std::unique_ptr<Expression> r;
	};



	struct Reflect final : public Expression {
		struct NamespaceQuery {
			Namespace namespaze;
		};
		
		struct FunctionQuery {
			Namespace namespaze;
			std::string name;
			std::size_t arity;
		};
		
		struct FileQuery {};
		
		struct LineQuery {};
		
		struct LocationQuery {
			FileQuery file;
			LineQuery line;
		};
		
		struct ExprQuery {
			std::unique_ptr<Expression> expr;
		};
		
		struct TypeQuery {
			type::IncompleteType type;
		};
		
		using Query = std::variant<
			NamespaceQuery,
			FunctionQuery,
			FileQuery,
			LineQuery,
			LocationQuery,
			ExprQuery,
			TypeQuery
		>;
		
		Reflect(
			Query query,
			const SourceLocation & location)
			: Expression(location)
			, query{std::move(query)} {}
		
		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		Query query;
	};



	struct TypedUnary final : public Expression {
		using Op = TypedUnaryOp;

		TypedUnary(
			Op op,
			const type::IncompleteType & type,
			std::unique_ptr<Expression> expr,
			const SourceLocation & location)
			: Expression{location}
			, op{op}
			, type{type}
			, expr{std::move(expr)} {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		Op op;
		type::IncompleteType type;
		std::unique_ptr<Expression> expr;
	};




	struct Integer final : public Expression {
		Integer(std::bitset<64> value, const SourceLocation & location)
			: Integer(static_cast<std::int64_t>(value.to_ullong()), location) {}

		Integer(std::int64_t value, const SourceLocation & location)
			: Expression(location), value(value) {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}
		
		std::int64_t value;
	};



	struct Float final : public Expression {
		Float(stx::float64_t value, const SourceLocation & location)
			: Expression(location), value(value) {}
		
		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		stx::float64_t value;
	};



	struct Bool final : public Expression {
		Bool(bool value, const SourceLocation & location)
			: Expression(location), value(value) {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		bool value;
	};



	struct Null final : public Expression {
		Null(const SourceLocation & location)
			: Expression(location) {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}
	};



	struct Char final : public Expression {
		Char(std::uint8_t value, const SourceLocation & location)
			: Expression(location), value(value) {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::uint8_t value;
	};



	struct String final : public Expression {
		String(const std::string & value, const SourceLocation & location)
			: Expression(location), value(value) {}
		
		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::string value;
	};



	struct Array final: public Expression {
		Array(
			const SourceLocation & location,
			std::vector<std::unique_ptr<Expression>> elements)
			: Expression(location)
			, elements{std::move(elements)} {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::vector<std::unique_ptr<Expression>> elements;
	};



	struct Tuple final: public Expression {
		Tuple(
			const SourceLocation & location,
			std::vector<std::unique_ptr<Expression>> elements)
			: Expression(location)
			, elements{std::move(elements)} {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::vector<std::unique_ptr<Expression>> elements;
	};



	struct Lambda final : public Expression {
		Lambda(
			std::unique_ptr<Function> fx,
			std::vector<std::unique_ptr<Var>> captures,
			const SourceLocation & location)
			: Expression(location)
			, fx(std::move(fx))
			, captures(std::move(captures)) {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::unique_ptr<Function> fx;
		std::vector<std::unique_ptr<Var>> captures;
	};


	
	struct Index final : public Expression {
		Index(
			std::unique_ptr<Expression> expr,
			std::unique_ptr<Expression> index,
			const SourceLocation & location)
			: Expression(location)
			, expr(std::move(expr))
			, index(std::move(index)) {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::unique_ptr<Expression> expr;
		std::unique_ptr<Expression> index;
	};



	struct Var final : public Expression {
		Var(
			const std::string & name,
			const Namespace & namespaze,
			const SourceLocation & location)
			: Expression(location)
			, name{name}
			, namespaze{namespaze} {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::string name;
		Namespace namespaze;
	};



	struct GlobalVar final : public Expression {
		GlobalVar(
			const SourceLocation & location,
			const Namespace & namespaze,
			const std::string & name)
			: Expression(location)
			, name { name }
			, namespaze { namespaze } {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::string name;
		Namespace namespaze;
	};



	struct Member final : public Expression {
		Member(
			std::unique_ptr<Expression> expr,
			const std::string & name,
			const SourceLocation & location)
			: Expression(location)
			, expr(std::move(expr))
			, name(std::move(name)) {};

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::unique_ptr<Expression> expr;
		std::string name;
	};



	struct Iife final : public Expression {
		Iife(
			const SourceLocation & location,
			std::unique_ptr<Statement> stmt,
			type::IncompleteType return_type) 
			: Expression(location)
			, stmt(std::move(stmt))
			, return_type{return_type} {}
			
		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::unique_ptr<Statement> stmt;
		type::IncompleteType return_type;
	};



	struct FxPointer final : public Expression {
		FxPointer(
			const std::string & name,
			const Namespace & namespaze,
			const std::size_t placeholders,
			const SourceLocation & location)
			: Expression(location)
			, name(name)
			, namespaze(namespaze)
			, placeholders(std::move(placeholders)) {}
		
		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::string name;
		Namespace namespaze;
		std::size_t placeholders;
		std::vector<type::IncompleteType> template_arguements;
	};



	struct Call final : public Expression {
		Call(
			std::unique_ptr<Expression> function_ptr,
			std::vector<std::unique_ptr<Expression>> arguments,
			const SourceLocation & location)
			: Expression(location)
			, function_ptr(std::move(function_ptr))
			, arguments(std::move(arguments)) {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::unique_ptr<Expression> function_ptr;
		std::vector<std::unique_ptr<Expression>> arguments;
		std::vector<type::IncompleteType> template_arguments;
	};



	struct InitStruct final : public Expression {
		struct Member {
			std::string name;
			std::unique_ptr<Expression> expr;
		};

		InitStruct(const SourceLocation & location)
			: Expression{location} {}
		
		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}

		std::vector<Member> members;
	};



	struct Choose final : Expression {
		Choose(const SourceLocation & location)
			: Expression{location} {}

		virtual void accept(const ExprVisitor & visitor) const override {
			visitor.visit(*this);
		}
		
		std::unique_ptr<Expression> condition;
		
		std::vector<std::pair<
			std::unique_ptr<Expression>,
			std::unique_ptr<Expression>
		>> cases;
		
		std::unique_ptr<Expression> d3fault;
	};



	auto visit_expression(const Expression & expr, auto && fx) {
		using Callable = std::decay_t<decltype(fx)>;
		using Ret = std::invoke_result_t<Callable, Binary>;
		using Base = FunctionVisitor<ExprVisitor, Callable, Ret>;

		struct Visitor : public Base {
			Visitor(Callable fx) : Base {fx} {} 

			virtual void visit(const Binary & x)             const override { this->run(x); };
			virtual void visit(const Unary & x)              const override { this->run(x); };
			virtual void visit(const Integer & x)            const override { this->run(x); };
			virtual void visit(const Float & x)              const override { this->run(x); };
			virtual void visit(const Bool & x)               const override { this->run(x); };
			virtual void visit(const Char & x)               const override { this->run(x); };
			virtual void visit(const Null & x)               const override { this->run(x); };
			virtual void visit(const String & x)             const override { this->run(x); };
			virtual void visit(const Array & x)              const override { this->run(x); };
			virtual void visit(const Tuple & x)              const override { this->run(x); };
			virtual void visit(const Call & x)               const override { this->run(x); };
			virtual void visit(const Var & x)                const override { this->run(x); };
			virtual void visit(const Index & x)              const override { this->run(x); };
			virtual void visit(const Lambda & x)             const override { this->run(x); };
			virtual void visit(const FxPointer & x)          const override { this->run(x); };
			virtual void visit(const Member & x)             const override { this->run(x); };
			virtual void visit(const GlobalVar & x)          const override { this->run(x); };
			virtual void visit(const Iife & x)               const override { this->run(x); };
			virtual void visit(const Ternary & x)            const override { this->run(x); };
			virtual void visit(const Choose & x)         const override { this->run(x); };
			virtual void visit(const TypedUnary & x)         const override { this->run(x); };
			virtual void visit(const Reflect & x)            const override { this->run(x); };
			virtual void visit(const ForwardDynamicCall & x) const override { this->run(x); };
			virtual void visit(const InitStruct & x)         const override { this->run(x); };
		};

		return Visitor{fx}(expr);
	}
}