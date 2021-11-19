#include "assemble.hxx"
#include "ltn/InstructionSet.hxx"
#include "unordered_map"
namespace ltn::a::assemble {
	namespace {
		const static std::unordered_map<std::string_view, Inst> instTabel {
			{"add", Inst::ADD},
			{"sub", Inst::SUB},
			{"mlt", Inst::MLT},
			{"div", Inst::DIV},
			{"mod", Inst::MOD},
			{"eql", Inst::EQL},
			{"ueql", Inst::UEQL},
			{"sml", Inst::SML},
			{"bgr", Inst::BGR},
			{"smleql", Inst::SMLEQL},
			{"bgreql", Inst::BGREQL},
			{"neg", Inst::NEG},

			{"newu", Inst::NEWU},
			{"newi", Inst::NEWI},
			{"newd", Inst::NEWF},
			{"true", Inst::TRUE},
			{"false", Inst::FALSE},
			{"read", Inst::READ},
			{"write", Inst::WRITE},
			{"scrap", Inst::SCRAP},

			{"jump", Inst::JUMP},
			{"link", Inst::LINK},
			{"return", Inst::RETURN},
			{"if", Inst::IF},
			{"error", Inst::ERROR},

			{"out", Inst::OUT},
		};


		std::uint8_t assembleLine(std::istream & in) {
			std::string inst;
			in >> inst;
			if(inst == "byte") {
				std::uint16_t byte;
				in >> std::hex >> byte;
				return static_cast<std::uint8_t>(byte);
			}
			if(instTabel.contains(inst)) {
				return static_cast<std::uint8_t>(instTabel.at(inst));
			}
			return 0;
		}
	}


	std::vector<std::uint8_t> assemble(std::istream & in) {
		std::vector<std::uint8_t> bytecode;
		while(!in.eof()) {
			bytecode.push_back(assembleLine(in));
		}
	}
}