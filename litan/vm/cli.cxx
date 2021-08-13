#include "LtnVM.hxx"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include "LtnFileIO.hxx"

void benchmark(const std::vector<std::uint64_t> & instructions, unsigned number){
	std::cout << ">> Creating virual machine..." << std::endl;
	ltn::vm::VM vm;
	std::cout << ">> Initialising vm" << std::endl;
	vm.init(instructions);
	std::cout << ">> Benchmarking " << number << "x" << std::endl;
	auto t0 = std::chrono::high_resolution_clock::now();
	for(unsigned i = 0; i < number; i++){
		vm.run();
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> delta = t1 - t0;
	std::cout << ">> Result: " << delta.count() << "ms" << std::endl;
}

std::vector<std::uint64_t> load(const std::string & file, const std::string & mode){
	std::string code = ltn::readFile(file);
	if(mode == "-hex") {
		return ltn::fromHex(code);
	}
	if(mode == "-bin") {
		return ltn::fromBin(code);
	}
	return {};
}

void launch(const std::vector<std::uint64_t> & instructions){

	std::cout << ">> Creating virual machine..." << std::endl;
	ltn::vm::VM vm;
	std::cout << ">> Initialising vm" << std::endl;
	vm.init(instructions);
	std::cout << ">> Starting vm" << std::endl;
	try {
		vm.run();
		std::cout << ">> Terminated" << std::endl;
	}
	catch(ltn::vm::RuntimeError error) {
		std::cout << ">> Error: " << error.what() << std::endl;
	}	
}

int main(int argc, char const *argv[]) {
	if(argc <= 2) return 1;
	std::cout << "Litan Virtual Machine (c) Jene Litsch 2021" << std::endl;

	std::string mode = argv[1];
	std::string file = argv[2];
	std::string format = "-hex";
	if(argc > 3) {
		format = argv[3];
	}
	
	try {
		if(mode == "-run"){
			launch(load(file, format));
		}

		if(mode.substr(0,7) == "-bench="){
			unsigned number;
			if(argc > 3) {
				number = static_cast<unsigned>(std::stoul(mode.substr(7)));
			}
			else{
				number = 1000;
			}

			benchmark(load(file, format),number);
		}
		std::cout << std::endl;
	}
	catch(const ltn::Error & error) {
		std::cout << ">> VM terminated after runtime error: ";
		std::cout << error << std::endl;
		std::cout << std::endl;
	}

	return 0;
}