#include "LtnVM.hxx"
#include "type_check.hxx"
#include "index.hxx"

namespace ltn::vm {
	void LtnVM::read() {
		const auto addr = this->reg.pop();
		const auto value = this->stack.read(addr.u);
		this->reg.push(value);
	}


	void LtnVM::write() {
		const auto addr = this->reg.pop();
		const auto value = this->reg.pop();
		this->stack.write(addr.u, value);
	}


	void LtnVM::scrap() {
		this->reg.pop();
	}

	void LtnVM::duplicate() {
		this->reg.push(this->reg.peek());
	}


	void LtnVM::makevar() {
		this->stack.make_var();
	}


	void LtnVM::read_x() {
		const auto addr = this->fetch_uint();
		const auto value = this->stack.read(addr);
		this->reg.push(value);
	}


	void LtnVM::write_x() {
		const auto addr = this->fetch_uint();
		const auto value = this->reg.pop();
		this->stack.write(addr, value);
	}


	void LtnVM::swap() {
		const auto a = this->reg.pop();
		const auto b = this->reg.pop();
		this->reg.push(a);
		this->reg.push(b);
	}


	void LtnVM::read_0() { this->reg.push(this->stack.read(0)); }
	void LtnVM::read_1() { this->reg.push(this->stack.read(1)); }
	void LtnVM::read_2() { this->reg.push(this->stack.read(2)); }
	void LtnVM::read_3() { this->reg.push(this->stack.read(3)); }

	void LtnVM::write_0() { this->stack.write(0, this->reg.pop()); }
	void LtnVM::write_1() { this->stack.write(1, this->reg.pop()); }
	void LtnVM::write_2() { this->stack.write(2, this->reg.pop()); }
	void LtnVM::write_3() { this->stack.write(3, this->reg.pop()); }
}