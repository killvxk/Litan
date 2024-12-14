#include "stack.hxx"
#include "ltnvm/objects/container/Stack.hxx"
#include "ltnvm/Exception.hxx"

namespace ltn::vm::stdlib {
	Value stack_size::func(Context *, const Value * args) {
		Stack * stack = req_stack(args + 0);
		return value::integer(stack->size());
	}

	Value stack_is_empty::func(Context * context, const Value * args) {
		return value::boolean(stack_size::func(context, args).i == 0);
	}

	Value stack_push::func(Context * context, const Value * args) {
		Stack * stack = req_stack(args + 0);
		stack->push_back(args[1]);
		return value::null;
	}

	Value stack_pop::func(Context * context, const Value * args) {
		Stack * stack = req_stack(args + 0);
		if (std::empty(*stack)) throw except::out_of_range();
		Value value = stack->unsafe_back();
		stack->unsafe_pop_back();
		return value;
	}
}