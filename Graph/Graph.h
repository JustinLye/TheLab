

#include<iostream>
#include<map>
#include<list>
#include<set>
#include<stdexcept>
#include<vector>
#include<queue>
#include"../Vertex/Vertex.h"

#if !defined(__JL_GRAPH_HEADER__)
#define __JL_GRAPH_HEADER__
namespace jl {
#if !defined(__JL_IDX_DEF__)
#define __JL_IDX_DEF__
	typedef std::size_t idx;
#endif
#if !defined(__JL_VERT_DEF__)
#define __JL_VERT_DEF__
	typedef std::pair<idx, std::set<idx>> vert;
#endif 
	class Graph {
	protected:
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
		inline bool IsGraphVertex(const idx& u) const { return (_g.find(u) != _g.end()); }

		inline std::vector<idx> Edges(const idx& u) const {
			std::vector<idx> e;
			if (IsGraphVertex(u)) {
				for (auto i : _g.at(u)) {
					e.push_back(i);
				}
			}
			return e;
		}
		inline std::vector<idx> Vertices() const {
			std::vector<idx> e;
			for (auto i : _g) {
				e.push_back(i.first);
			}
			return e;
		}
		inline void Clear() {
			_g.clear();
			next_id = start_id;
		}
		inline jl::idx StartId() const { return this->start_id; }
		friend std::ostream& operator<<(std::ostream& o, const Graph& G) {
			G.output(o);
			return o;
		}
	};
	typedef std::map<jl::idx, jl::Vertex_BFS> bfsmap;
	typedef std::map<jl::idx, jl::Vertex_BFS*> bfsmapptr;
	typedef std::pair<jl::idx, jl::Vertex_BFS> bfsmap_pair;
	typedef std::pair<jl::idx, jl::Vertex_BFS*> bfsmapptr_pair;
	typedef std::vector<jl::Vertex_BFS> bfsvec;
	struct BFS_Tree {
		bfsvec vertices;
		BFS_Tree(const bfsvec& v) : vertices(v) {}
		friend std::ostream& operator<<(std::ostream& o, const BFS_Tree& b) {
			for (auto i : b.vertices)
				o << i << std::endl;
			return o;
		}
	};
	class BFS_Tree_Factory {
	private:
		jl::Graph const *_g;
		jl::idx _s;
		jl::idx _sid;
		bfsvec _v;
		void _init() {
			_v.clear();
			for (auto i : _g->Vertices()) {
				try {
					_v.push_back(static_cast<jl::Vertex_BFS>(i));
				}
				catch (...) {
					std::cerr << "Error" << std::endl;
				}
				if (i == _s) {
					_v.back().color = BLACK;
					_v.back().distance = 0;
				}
			}
		}
		void _search() {
			std::queue<jl::Vertex_BFS*> _q;
			_q.push(&_v[_s - _sid]);
			while (!_q.empty()) {
				jl::Vertex_BFS* _u = _q.front();
				_q.pop();
				for (auto i : _g->Edges(*_u)) {
					if (_v[i-_sid].color == WHITE) {
						_v[i-_sid].color = BLACK;
						_v[i-_sid].distance = _v[*(_u) - _sid].distance + 1;
						_v[i-_sid].predecessor = &_v[*(_u) - _sid];
						_q.push(&_v[i - _sid]);
					}
				}
			}

		}
	public:
		BFS_Tree operator()(const Graph& G, const idx& s) {
			_g = &G;
			_s = s;
			_sid = _g->StartId();
			_init();	
			_search();
			return BFS_Tree(_v);
		}

		

	};

}
#endif