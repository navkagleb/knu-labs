#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

//int main(int argc, char** argv) {
//
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//
//}

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

#include "DataStructures/Array.hpp"
#include "DataStructures/Graphs/MatrixGraph.hpp"
#include "DataStructures/Graphs/ListGraph.hpp"
#include "DataStructures/Trees/GeneralTree.hpp"
#include "FileSystem/FileSystem.hpp"
#include "DataStructures/Trees/ExpressionTree.hpp"

struct Vertex {

	int x;
	int y;

	bool operator<(const Vertex& other) const {
	    return x * y < other.x * other.y;
	}

	bool operator==(const Vertex& other) const {
	    return this->x == other.x && this->y == other.y;
	}

};

struct Edge {
    float value;
    std::string name;

    friend std::ostream& operator<<(std::ostream& ostream, const Edge& edge) {
        return ostream << edge.name << " -> " << edge.value;
    }
};

int main() {

    std::map<std::string, int> map;

//    ng::GeneralTree<int> tree(0);
//
//    tree.push(32);
//    tree.push(85);
//    tree.push(32, { 0 });
//
//    tree.tprint();

//    ng::FileSystem fileSystem;
//
//    fileSystem.import("fas");
//    fileSystem.printTree(fileSystem.root());

    ng::ExpressionTree tree;
//    std::cin >> tree;

    tree.input();
    tree.print();

//	ng::MatrixGraph<Vertex> graph(false, true);
//
//	Vertex a = { 0, 1 };
//	Vertex b = { 2, 3 };
//	Vertex c = { 4, 5 };
//	Vertex d = { 6, 7 };
//
//	graph.pushNode(a);
//	graph.pushNode(b);
//	graph.pushNode(c);
//	graph.pushNode(d);
//
//	graph.pushEdge(a, b, 3);
//	graph.pushEdge(a, c, 7);
//	graph.pushEdge(b, d, 8);
//
//	std::function<float(const int&)> f = [](const int& e) { return e; };
//
//	auto distance = graph.dijkstra(a);
//
//	for (const auto& p : distance)
//	    std::cout << "{ " << p.first.x << ", " << p.first.y << " } -> " << *p.second << std::endl;

}