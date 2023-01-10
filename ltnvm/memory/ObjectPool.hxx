#pragma once
#include <vector>
#include <optional>
#include <cstdint>
#include <queue>

namespace ltn::vm {
	template<typename T>
	class ObjectPool {
		struct Bundle {
			T obj;
			bool in_use = false;
			bool marked = false;
		};
	public:

		T & get(std::uint64_t id) {
			return objects[id].obj;
		}



		std::uint64_t alloc(T && obj) {
			if(reuse.empty()) {
				this->objects.push_back(Bundle {
					.obj = std::move(obj),
					.in_use = true,
					.marked = false
				});
				return this->next_id++;
			}
			else {
				const auto id = this->reuse.front();
				this->reuse.pop();
				this->objects[id] = Bundle {
					.obj = std::move(obj),
					.in_use = true,
					.marked = false
				};
				return id;
			}
		}



		void gc_mark(std::uint64_t id) {
			objects[id].marked = true;
		}



		void gc_sweep() {
			std::uint64_t id = 0;
			for(auto & [obj, in_use, marked] : objects) {
				if(!marked) {
					in_use = false;
					reuse.push(id);
				}
				else {
					marked = false;
				}
				++id;
			}
		}



		void clear() {
			this->objects.clear();
			this->reuse = {};
			this->next_id = 0;
		}



		bool gc_is_marked(std::uint64_t id) const {
			return objects[id].marked;
		}
	private:

		std::vector<Bundle> objects;
		std::queue<std::uint64_t> reuse;
		std::uint64_t next_id;
	};
}