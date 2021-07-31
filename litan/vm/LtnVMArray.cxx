#include "LtnVM.hxx"
#include "LtnAccessViolation.hxx"
#include "LtnIndexAccessViolation.hxx"
#include "LtnPointerAccessViolation.hxx"
#include "LtnPopFromEmpty.hxx"

// Array instructions
void ltn::VM::arrayClear(){
	std::uint64_t ptr = this->env.acc.popU();
	this->env.heap.access<HeapArray>(ptr).clear();
}

void ltn::VM::arrayGet(){
	std::uint64_t idx = this->env.acc.popU();
	std::uint64_t ptr = this->env.acc.popU();
	auto & array = this->env.heap.access<HeapArray>(ptr);
	if(idx < array.size()){
		this->env.acc.push(array[idx]);
	}
	else{
		throw IndexAccessViolation(ptr, idx, "Array");
	}
}

void ltn::VM::arraySet(){
	std::uint64_t value = this->env.acc.popU();
	std::uint64_t idx = this->env.acc.popU();
	std::uint64_t ptr = this->env.acc.popU();
	auto & array = this->env.heap.access<HeapArray>(ptr);
	if(idx < array.size()){
		array[idx] = value;
	}
	else{
		throw IndexAccessViolation(ptr, idx, "Array");
	}
}


void ltn::VM::arraySize(){
	std::uint64_t ptr = this->env.acc.popU();
	this->env.acc.push(this->env.heap.access<HeapArray>(ptr).size());
}

void ltn::VM::arrayEmpty(){
	std::uint64_t ptr = this->env.acc.popU();
	this->env.acc.push(this->env.heap.access<HeapArray>(ptr).empty());
}

void ltn::VM::arrayFill() {
	std::uint64_t value = this->env.acc.popU();
	std::uint64_t ptr = this->env.acc.popU();
	std::vector<std::uint64_t> & array = this->env.heap.access<HeapArray>(ptr);
	std::fill(array.begin(), array.end(), value);
}

void ltn::VM::arrayResize(){
	std::uint64_t size = this->env.acc.popU();
	std::uint64_t ptr = this->env.acc.popU();
	this->env.heap.access<HeapArray>(ptr).resize(size, 0);
}

void ltn::VM::arrayErase(){
	std::uint64_t idx = this->env.acc.popU();
	std::uint64_t ptr = this->env.acc.popU();
	std::vector<std::uint64_t> & array = this->env.heap.access<HeapArray>(ptr);
	array.erase(array.begin() + long(idx));
}

void ltn::VM::arrayInsert(){
	std::uint64_t value = this->env.acc.popU();
	std::uint64_t idx = this->env.acc.popU();
	std::uint64_t ptr = this->env.acc.popU();
	std::vector<std::uint64_t> & array = this->env.heap.access<HeapArray>(ptr);
	array.insert(array.begin() + long(idx), value);
}

void ltn::VM::arrayPushBack(){
	std::uint64_t value = this->env.acc.popU();
	std::uint64_t ptr = this->env.acc.popU();
	auto & array = this->env.heap.access<HeapArray>(ptr);
	array.push_back(value);
}


void ltn::VM::arrayPopBack(){
	std::uint64_t ptr = this->env.acc.popU();
	auto & array = this->env.heap.access<HeapArray>(ptr);
	if(array.empty()) {
		throw PopFromEmpty(ptr);
	}
	this->env.acc.push(array.back());
	array.pop_back();
}

void ltn::VM::arrayFront(){
	std::uint64_t ptr = this->env.acc.popU();
	auto & array = this->env.heap.access<HeapArray>(ptr);
	this->env.acc.push(array.front());
}

void ltn::VM::arrayBack(){
	std::uint64_t ptr = this->env.acc.popU();
	auto & array = this->env.heap.access<HeapArray>(ptr);
	this->env.acc.push(array.back());
}