#include "compile.hxx"
#include "stdxx/iife.hxx"
#include <string_view>
#include <iostream>
#include <filesystem>
namespace ltn::c {
	namespace {
		InstructionBuffer add_member(const std::size_t address,
			const InstructionBuffer & init) {
	
			InstructionBuffer buf;
			buf << inst::duplicate();
			buf << init;
			buf << inst::swap();
			buf << inst::member_write(address);
			return buf;
		}



		InstructionBuffer compile_reflect_query(
			const sst::Reflect::Addr & address,
			const sst::Reflect::FunctionQuery & query) {

			InstructionBuffer buf;
			buf << inst::newstruct();
			buf << add_member(address.name, {
				inst::newstr(query.name)
			});
			buf << add_member(address.full_name, {
				inst::newstr(query.full_name)
			});
			buf << add_member(address.fx_ptr, {
				inst::newfx(query.id, query.arity)
			});
			buf << add_member(address.c0nst, {
				query.c0nst ? inst::bool_true() : inst::bool_false()
			});

			buf << add_member(address.pr1vate, {
				query.pr1vate ? inst::bool_true() : inst::bool_false()
			});

			buf << add_member(address.ext3rn, {
				query.ext3rn ? inst::bool_true() : inst::bool_false()
			});
			return buf;
		}



		InstructionBuffer compile_reflect_query(
			const sst::Reflect::Addr & address,
			const sst::Reflect::NamespaceQuery & query) {

			InstructionBuffer buf;
			buf << inst::newstruct();
			buf << add_member(address.name, {
				inst::newstr(query.namespaze.to_string())
			});
			buf << add_member(address.functions, stx::iife([&] {
				InstructionBuffer buf;
				for(const auto & fx : query.functions) {
					buf << compile_reflect_query(address, fx);
				}
				buf << inst::newarr(query.functions.size());
				return buf;
			}));
			return buf;
		}



		InstructionBuffer compile_reflect_query(
			const sst::Reflect::Addr &,
			const sst::Reflect::LineQuery & query) {

			InstructionBuffer buf;
			buf << inst::newi(static_cast<std::int64_t>(query.line));
			return buf;
		}



		InstructionBuffer compile_reflect_query(
			const sst::Reflect::Addr & address,
			const sst::Reflect::FileQuery & query) {

			const auto path = std::filesystem::path{query.name};

			InstructionBuffer buf;
			buf << inst::newstruct();
			buf << add_member(address.name, {
				inst::newstr(path.filename().string())
			});
			return buf;
		}



		InstructionBuffer compile_reflect_query(
			const sst::Reflect::Addr & address,
			const sst::Reflect::LocationQuery & query) {

			InstructionBuffer buf;
			buf << inst::newstruct();
			buf << add_member(address.line, compile_reflect_query(address, query.line));
			buf << add_member(address.file, compile_reflect_query(address, query.file));
			return buf;
		}



		InstructionBuffer compile_reflect_query(
			const sst::Reflect::Addr & address,
			const sst::Reflect::TypeQuery & query) {

			InstructionBuffer buf;
			buf << inst::newstruct();
			buf << add_member(address.name, {
				inst::newstr(type::to_string(query.type))
			});
			return buf;
		}



		InstructionBuffer compile_reflect_query(
			const sst::Reflect::Addr & address,
			const sst::Reflect::ExprQuery & query) {

			InstructionBuffer buf;
			buf << inst::newstruct();
			buf << add_member(address.type, compile_reflect_query(address, query.type_query));
			return buf;
		}
	}



	// compiles array literal
	InstructionBuffer compile_expr(const sst::Reflect & refl) {

		return std::visit([&] (const auto & query) {
			return compile_reflect_query(refl.address, query);
		}, refl.query);
	}
}