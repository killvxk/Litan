#include <fstream>
#include <filesystem>
#include "ltnvm/LtnVM.hxx"
#include "ltnvm/external/External.hxx"
#include "ltnvm/cast.hxx"
#include "ltn/version.hxx"
#include "ltn/args.hxx"
#include "stdxx/args.hxx"


std::vector<std::uint8_t> read_bytecode(const std::filesystem::path & path) {
	std::ifstream file(path, std::ios::binary);
	if(!file) {
		std::ostringstream oss;
		oss << "Cannot open " << path; 
		throw std::runtime_error { oss.str() }; 
	}
	return std::vector<std::uint8_t> {
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>()
	};
}


int main(int argc, char const *argv[]) {
	stx::args args {argc, argv};

	stx::option_description desc {
		"Litan Virtual Machine [ltnvm] " + std::string{ltn::version},
		"The Litan VM executes an previous compiled Litan bytecode file."
	};

	auto & exec = desc.add<stx::option_string>(
		{"--exe"},
		"Executable Bytecode",
		"Path to an executable Litan bytecode file");

	auto & flag_version = ltn::args::version(desc);
	auto & flag_help    = ltn::args::help(desc);
	auto & main_args    = ltn::args::main_args(desc);

	args.parse_options(desc);

	if(flag_version.is_set()) {
		std::cout << "Litan: " << ltn::version << "\n";
		return EXIT_SUCCESS;
	}
	if(flag_help.is_set()) {
		std::cout << desc.describe(); 
		return EXIT_SUCCESS;
	}

	exec.mandatory();

	try {
		const auto bytecode = read_bytecode(exec.get());

		ltn::vm::LtnVM vm;
		vm.setup(bytecode);
		auto x = vm.run(main_args.get());
		std::cout
			<< "Exit main() with return value: "
			<< ltn::vm::cast::to_string(x, vm.get_heap())
			<< "\n";
		
		return EXIT_SUCCESS;
	}
	catch(const std::runtime_error & error) {
		std::cout << "[VM-Error] " << error.what() << "\n";
		return EXIT_FAILURE;
	}
}
