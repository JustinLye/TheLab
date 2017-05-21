

#include<iostream>
#include<map>
#include<list>
#include<set>
#include<stdexcept>
#if !defined(__JL_GRAPH_HEADER__)
#define __JL_GRAPH_HEADER__
namespace jl {
	typedef std::size_t idx;
	typedef std::pair<idx, std::set<idx>> vert;
	class Graph {
	private:
		idx start_id;
		idx next_id;
		std::map<idx, std::set<idx>> _g;
		inline void output(std::ostream& o) const {
			for (auto i : _g) {
				o << i.first << ' ';
				for (auto j : i.second)
					o << j << "->";
				o << '\\' << std::endl;
			}
		}
		inline bool validate(const idx& u, const idx& v) {
			return (_g.find(u) != _g.end() && _g.find(v) != _g.end());
		}
	public:
		Graph(const idx& i = 0) : start_id(i), next_id(i) {}
		inline void Insert() {
			_g.insert(vert(next_id++, std::set<idx>()));
		}
		inline void Connect(const idx& u, const idx& v) {
			if (!validate(u, v))
				std::cerr << "Could not connect (" << u << ", " << v <<  ") because u and/or v are not in the graph" << std::endl;
			else {
				_g[u].insert(v);
			}
		}
		inline void Connect(const idx& u, std::initializer_list<idx> v) {
			for (auto i : v)
				this->Connect(u, i);
		}
		inline void Connect(std::initializer_list<idx> u, std::initializer_list<std::initializer_list<idx>> v) {
			auto i = u.begin();
			auto j = v.begin();
			while (i != u.end() && j != v.end()) {
				for (auto p : *j) {
					this->Connect(*i, p);
				}
				i++;
				j++;
			}
		}
		inline void Clear() {
			_g.clear();
			next_id = start_id;
		}
		friend std::ostream& operator<<(std::ostream& o, const Graph& G) {
			G.output(o);
			return o;
		}
	};
}
#endif