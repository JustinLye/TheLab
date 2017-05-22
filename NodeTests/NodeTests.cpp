#include<gtest/gtest.h>
#include<chrono>
#include"../Graph/Graph.h"



class NodeTest : public ::testing::Test {
protected:
	jl::Graph G;
	jl::BFS_Tree_Factory btf;
	virtual void SetUp() {
		G = jl::Graph(1);
	}
	virtual void TearDown() {
	}
	virtual void Insertion(const jl::idx& n = 6) {
		G.Clear();
		for (jl::idx i = 0; i < n; i++) {
			G.Insert();
		}
	}
	virtual void MakeSample() {
		Insertion();
		G.Connect(1, { 2,4 });
		G.Connect(2, 5);
		G.Connect(3, { 6,5 });
		G.Connect(4, 2);
		G.Connect(5, 4);
		G.Connect(6, 6);
	}
};

TEST_F(NodeTest, Init) {
	std::cout << G << std::endl;
}
TEST_F(NodeTest, Inserting) {
	G.Clear();
	for (jl::idx i = 0; i < 6; i++) {
		G.Insert();
	}
	std::cout << G << std::endl;
}
TEST_F(NodeTest, Connecting) {
	Insertion();
	G.Connect(1, 2);
	G.Connect(0, 3);
	G.Connect(1, 4);
	std::cout << G << std::endl;
}

TEST_F(NodeTest, Connecting_InitList) {
	Insertion();
	G.Connect(0, { 1,2 });
	std::cout << G << std::endl;
}

TEST_F(NodeTest, BuildSample) {
	MakeSample();
	std::cout << G << std::endl;
}
TEST_F(NodeTest, Connecting_DblInitList) {
	Insertion();
	G.Connect({ 1,2,3,4,5,6 }, { {2,4},{5},{6,5},{2},{4}, {6} });
	std::cout << G << std::endl;
}

TEST_F(NodeTest, EdgeTest) {
	MakeSample();
	std::vector<jl::idx> e = G.Edges(1);
	for (auto i : e) {
		std::cout << i << std::endl;
	}
}

TEST_F(NodeTest, BFSTest) {
	MakeSample();
	std::cout << btf(G, 3) << std::endl;
}
TEST_F(NodeTest, BFSTest2) {
	Insertion(8);
	G.Connect({ 1,2,3,4,5,6,7,8 }, { { 2,5 }, { 1,6 }, { 4,6,7 }, { 3,7,8 }, { 1 }, { 2,3,7 }, { 3,4,6,8 }, { 4,7 } });
	std::cout << btf(G, 2) << std::endl;

}


int main(int argc, char* argv[]) {

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}