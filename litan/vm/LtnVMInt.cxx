#include "LtnVM.hxx"
#include <cmath>

// Math Instruction
void ltn::vm::VM::addi(){
	const std::int64_t r = this->env.acc.popI();
	const std::int64_t l = this->env.acc.popI();
	this->env.acc.push(l + r);
}

void ltn::vm::VM::subi(){
	const std::int64_t r = this->env.acc.popI();
	const std::int64_t l = this->env.acc.popI();
	this->env.acc.push(l - r);
}

void ltn::vm::VM::mlti(){
	const std::int64_t r = this->env.acc.popI();
	const std::int64_t l = this->env.acc.popI();
	this->env.acc.push(l * r);
}

void ltn::vm::VM::divi(){
	const std::int64_t r = this->env.acc.popI();
	const std::int64_t l = this->env.acc.popI();
	this->env.acc.push(l / r);
}

void ltn::vm::VM::powi(){
	const std::int64_t r = this->env.acc.popI();
	const std::int64_t l = this->env.acc.popI();
	this->env.acc.push(static_cast<std::int64_t>(std::pow(l, r)));
}

void ltn::vm::VM::modi(){
	const std::int64_t r = this->env.acc.popI();
	const std::int64_t l = this->env.acc.popI();
	this->env.acc.push(l % r);
}

void ltn::vm::VM::inci(){
	this->env.acc.top()++;
}

void ltn::vm::VM::deci(){
	this->env.acc.top()--;
}

void ltn::vm::VM::mnsi(){
	this->env.acc.push(-this->env.acc.popI());
}

void ltn::vm::VM::mini(){
	const std::int64_t a = this->env.acc.popI();
	const std::int64_t b = this->env.acc.popI();
	this->env.acc.push(std::min(a, b));
}

void ltn::vm::VM::maxi(){
	const std::int64_t a = this->env.acc.popI();
	const std::int64_t b = this->env.acc.popI();
	this->env.acc.push(std::max(a, b));
}



// Comparison
void ltn::vm::VM::eqli(){
	const std::uint8_t invert = this->getArg8();
	const std::int64_t r = this->env.acc.popI();
	const std::int64_t l = this->env.acc.popI();
	const bool result = (l == r) != (invert & 0x01);
	this->env.acc.push(result);
}

void ltn::vm::VM::smli(){
	const std::int64_t r = this->env.acc.popI();
	const std::int64_t l = this->env.acc.popI();
	this->env.acc.push(l < r);
}

void ltn::vm::VM::bgri(){
	const std::int64_t r = this->env.acc.popI();
	const std::int64_t l = this->env.acc.popI();
	this->env.acc.push(l > r);
}

void ltn::vm::VM::spshi(){
	const std::int64_t r = this->env.acc.popI();
	const std::int64_t l = this->env.acc.popI();
	if(l == r) return this->env.acc.push(0L);
	if(l > r) return this->env.acc.push(1L);
	if(l < r) return this->env.acc.push(-1L);
}