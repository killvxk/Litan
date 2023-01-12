#include "Heap.hxx"
#include "ltnvm/type_check.hxx"
namespace ltn::vm {
	void Heap::collect_garbage(const Stack & stack, const std::vector<Value> & globals) {
		if(gc_counter >= gc_frequency) {
			mark(stack.get_values());
			mark(globals);
			sweep();
			gc_counter = 0;
		}
		else {
			++gc_counter;
		}
	}



	void Heap::mark(const std::vector<Value> & values) {
		for(const auto & value : values) {
			this->mark(value);
		}
	}



	void Heap::mark(const std::deque<Value> & values) {
		for(const auto & value : values) {
			this->mark(value);
		}
	}



	void Heap::mark(const Value & value) {
		if(is_array(value))   return this->mark_array(value);
		if(is_string(value))  return pool_of<String>().gc_mark(value.u);
		
		if(is_istream(value)) return pool_of<IStream>().gc_mark(value.u);
		if(is_ostream(value)) return pool_of<OStream>().gc_mark(value.u);
		
		if(is_fxptr(value))   return this->mark_fxptr(value);

		if(is_clock(value))   return pool_of<Clock>().gc_mark(value.u);
		
		if(is_struct(value))  return this->mark_struct(value);

		if(is_stack(value))   return this->mark_deque(value);
		if(is_queue(value))   return this->mark_deque(value);

		if(is_map(value))     return this->mark_map(value);
		
		if(is_rng(value))     return pool_of<RandomEngine>().gc_mark(value.u);
	}



	void Heap::mark_array(const Value & ref) {
		auto & pool = pool_of<Array>(); 
		if(!pool.gc_is_marked(ref.u)) {
			pool.gc_mark(ref.u);
			auto & arr = pool.get(ref.u).get();
			this->mark(arr);
		}
	}



	void Heap::mark_fxptr(const Value & ref) {
		auto & pool = pool_of<FxPointer>(); 
		if(!pool.gc_is_marked(ref.u)) {
			pool.gc_mark(ref.u);
			auto & fx = pool.get(ref.u);
			this->mark(fx.captured);
		}
	}



	void Heap::mark_struct(const Value & ref) {
		auto & pool = pool_of<Struct>(); 
		if(!pool.gc_is_marked(ref.u)) {
			pool.gc_mark(ref.u);
			auto & s = pool.get(ref.u);
			for(const auto & [key, value] : s.members) {
				this->mark(value);
			}
		}
	}



	void Heap::mark_deque(const Value & ref) {
		auto & pool = pool_of<Deque>(); 
		if(!pool.gc_is_marked(ref.u)) {
			pool.gc_mark(ref.u);
			auto & deq = pool.get(ref.u).get();
			this->mark(deq);
		}
	}



	void Heap::mark_map(const Value & value) {
		auto & pool = pool_of<Map>(); 
		if(!pool.gc_is_marked(value.u)) {
			pool.gc_mark(value.u);
			auto & map = pool.get(value.u).get();
			for(auto & [key, value] : map) {
				this->mark(key);
				this->mark(value);
			}
		}
	}



	void Heap::sweep() {
		pool_of<String>().gc_sweep();
		pool_of<Array>().gc_sweep();
		pool_of<IStream>().gc_sweep();
		pool_of<OStream>().gc_sweep();
		pool_of<FxPointer>().gc_sweep();
		pool_of<Clock>().gc_sweep();
		pool_of<Struct>().gc_sweep();
		pool_of<Deque>().gc_sweep();
		pool_of<Map>().gc_sweep();
		pool_of<RandomEngine>().gc_sweep();
	}
}