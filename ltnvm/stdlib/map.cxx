#include "map.hxx"
#include "ltnvm/native/native.hxx"
#include "ltnvm/Exception.hxx"
#include "ltnvm/VMCore.hxx"

namespace ltn::vm::stdlib {
	Value map_size::func(Context *, const Value * args) {
		Map * map = req_map(args + 0);
		return value::integer(std::size(*map));
	}



	Value map_is_empty::func(Context *, const Value * args) {
		Map * map = req_map(args + 0);
		return value::integer(std::empty(*map));
	}



	Value map_at::func(Context *, const Value * args) {
		Map * map = req_map(args + 0);
		const Value key = args[1];
		return map->at(key);
	}



	Value map_has::func(Context *, const Value * args) {
		Map * map = req_map(args + 0);
		const Value key = args[1];
		return value::boolean(map->contains(key));
	}



	Value map_insert::func(Context *, const Value * args) {
		Map * map = req_map(args + 0);
		const Value key = args[1];
		const Value value = args[2];
		(*map)[key] = value;
		return value::null;
	}



	Value map_erase::func(Context * context, const Value * args) {
		Map * map = req_map(args + 0);
		const Value key = args[1];
		const Value value = map_at::func(context, args);
		map->erase(key);
		return value;
	}



	Value map_keys::func(Context * context, const Value * args) {
		VMCore & core = *static_cast<VMCore*>(context->core);
		Map * map = req_map(args + 0);
		Array * keys = core.heap.alloc(map->keys());
		return value::array(keys);
	}



	Value map_values::func(Context * context, const Value * args) {
		VMCore & core = *static_cast<VMCore*>(context->core);
		Map * map = req_map(args + 0);
		Array * values = core.heap.alloc(map->values());
		return value::array(values);
	}



	Value map_merged::func(Context * context, const Value * args) {
		VMCore & core = *static_cast<VMCore*>(context->core);
		Map * map_l = req_map(args + 0);
		Map * map_r = req_map(args + 1);
		Map * merged_map = core.heap.alloc(map_l->merged(*map_r));
		return value::map(merged_map);
	}
}