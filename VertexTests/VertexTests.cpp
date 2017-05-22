#include<gtest/gtest.h>
#include"../Vertex/Vertex.h"


class VertexTest : public ::testing::Test {
protected:
	jl::BasicVertex v0;
	jl::BasicVertex v1;
	virtual void SetUp() {
		v1 = jl::BasicVertex(1);
	}
	virtual void TearDown() {
	}
};

class Vertex_BFSTest : public ::testing::Test{
protected:
	jl::Vertex_BFS v0;
	jl::Vertex_BFS v1;
	virtual void SetUp() {
		v0 = jl::Vertex_BFS(1);
		v1 = jl::Vertex_BFS(2);
	}
	virtual void TearDown() {

	}
};

TEST_F(VertexTest, Init) {
	EXPECT_EQ(v0, 0);
	EXPECT_EQ(v1, 1);
}

TEST_F(VertexTest, Comparison) {
	EXPECT_LE(v0, v1);
	EXPECT_GT(v1, v0);
	EXPECT_GE(v0, 0);
}

TEST_F(VertexTest, IntConversion) {
	int n = v1;
	EXPECT_EQ(n, 1);
	EXPECT_GT(n, v0);
}

TEST_F(Vertex_BFSTest, Init) {
	EXPECT_EQ(v0.color, jl::WHITE);
	EXPECT_EQ(v0.predecessor, nullptr);
	EXPECT_EQ(v0.distance, jl::INFINITE);

}
TEST_F(Vertex_BFSTest, Output) {
	std::cout << v0 << std::endl;
}

TEST_F(Vertex_BFSTest, KeyOrdering) {
	v1.distance = 1;
	v0.distance = 0;
	std::set<jl::Vertex_BFS, jl::CompareDist> s;
	s.insert(v1);
	s.insert(v0);
	auto a = s.begin();
	auto b = s.begin();
	b++;
	EXPECT_LE(*a, *b);
	for (auto i : s)
		std::cout << i << std::endl;

}

int main(int argc, char* argv[]) {

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}