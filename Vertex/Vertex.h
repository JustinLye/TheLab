#include<iostream>
#if !defined(__JL_VERTEX_HEADER__)
#define __JL_VERTEX_HEADER__

namespace jl {
#if !defined(__JL_IDX_DEF__)
#define __JL_IDX_DEF__
	typedef std::size_t idx;
#endif
	class BasicVertex {
	protected:
		idx id;
		virtual void output(std::ostream& o) const {
			o << id;
		}
	public:
		explicit BasicVertex(const idx& Id = 0) : id(Id) {}
		inline virtual bool operator<(const BasicVertex& v) const { return this->id < v.id; }
		inline virtual bool operator<=(const BasicVertex& v) const { return this->id <= v.id; }
		inline virtual bool operator==(const BasicVertex& v) const { return this->id == v.id; }
		inline virtual bool operator>(const BasicVertex& v) const { return this->id > v.id; }
		inline virtual bool operator>=(const BasicVertex& v) const { return this->id >= v.id; }
		inline virtual bool operator!=(const BasicVertex& v) const { return this->id != v.id; }
		inline virtual bool operator<(const idx& v) const { return this->id < v; }
		inline virtual bool operator<=(const idx& v) const { return this->id <= v; }
		inline virtual bool operator==(const idx& v) const { return this->id == v; }
		inline virtual bool operator>(const idx& v) const { return this->id > v; }
		inline virtual bool operator>=(const idx& v) const { return this->id >= v; }
		inline virtual bool operator!=(const idx& v) const { return this->id != v; }

		inline operator int() { return this->id; }
		inline operator idx() { return this->id; }
		inline operator const idx() const { return this->id; }
	};
	enum COLOR {WHITE, BLACK};
	const int INFINITE = std::numeric_limits<int>::max();
	class Vertex_BFS : public BasicVertex {
	protected:
		virtual void output(std::ostream& o) const {
			o << '(' << id << ", " << (color == WHITE ? "WHITE" : color == BLACK ? "BLACK" : "UNDEFINED COLOR") << ", " << distance << ", ";
			if (predecessor == nullptr)
				o << "NULL)";
			else
				o << predecessor->id << ')';
		}
	public:
		COLOR color;
		int distance;
		Vertex_BFS *predecessor;
		explicit Vertex_BFS(const idx& i = 0) : BasicVertex(i), color(WHITE), distance(INFINITE), predecessor(nullptr) {}
		explicit inline operator const idx() const { return this->id; }
		explicit inline operator std::size_t() const { return this->id; }
		friend inline std::ostream& operator<<(std::ostream& o, const Vertex_BFS& v) {
			v.output(o);
			return o;
		}
	};

	struct CompareDist {
		bool operator()(const Vertex_BFS& a, const Vertex_BFS& b) { return a.distance <= b.distance; }
	};
};



#endif