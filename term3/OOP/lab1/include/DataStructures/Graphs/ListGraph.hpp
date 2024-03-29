#pragma once

#include <vector>
#include <map>
#include <initializer_list>
#include <functional>
#include <stack>
#include <iostream>
#include <queue>
#include <functional>
#include <unordered_set>

#include "Graph.hpp"

namespace ng {

    template <typename N, typename E = int>
    class ListGraph : public Graph<N, E> {
    private:
        struct Edge;

    public:
        // constructor / destructor
        ListGraph();
        ListGraph(bool directed, bool weighed);
        ~ListGraph() override;

        // accessors
        [[nodiscard]] bool empty() const override;
        [[nodiscard]] std::size_t nodes() const override;
        [[nodiscard]] const int& edges() const override;
        [[nodiscard]] const bool& directed() const override;
        [[nodiscard]] const bool& weighed() const override;
        [[nodiscard]] const std::map<N, std::vector<Edge>>& list() const;

        [[nodiscard]] bool connected() const override;
        [[nodiscard]] std::vector<std::vector<N>> components() const override;

        // modifiers
        void directed(const bool& directed) override;
        void weighed(const bool& weighed) override;

        // public methods
        void pushNode(const N &value) override;
        void popNode(const N& value) override;
        void pushEdge(const N& from, const N& to, const E& weight) override;
        void pushEdge(const N& from, const N& to) override;
        void popEdge(const N& from, const N& to) override;
        void popEdges() override;
        void clear() override;
        void print() const override;

        void dfs(const N& node, bool* visited) const override;
        void dfs(const N& node, std::vector<N>& path) const override;
        std::vector<N> dfs(const N& node) const override;

        template <typename T = E>
        void bfs(N node, std::map<N, T>& distance, std::function<T(const E&)> f = [](const E& e) { return e; }) const;
        template <typename T = E>
        std::map<N, T> bfs(N node, std::function<T(const E&)> f = [](const E& e) { return e; }) const;

        template <typename T = E>
        std::map<N, T*> dijkstra(N node, std::function<T(const E&)> f = [](const E& e) { return e; });
//
//		[[nodiscard]] std::vector<int> kahn() const override;
//		[[nodiscard]] std::vector<int> topologicalSort() const override;
//
//		[[nodiscard]] SpanningTree* spanningTree(int snode) const override;
//		[[nodiscard]] std::vector<ng::SpanningTree*> spanningForest() const override;
//		[[nodiscard]] std::vector<ng::SpanningTree*> kruskal() const override;

    private:
        // inner structs
        struct Edge {
            // variables
            N toNode;
            E value;

            // constructor
            Edge() = default;
            Edge(N toNode, E value) : toNode(toNode), value(value) {}

        };

        // variables
        std::map<N, std::vector<Edge>> _list;
        std::map<N, int> _nodes;
        int _edges;
        bool _directed;
        bool _weighed;

        // private methods
        void _dfs(const N &node, bool *visited, std::vector<N> *path) const;

    }; // class MatrixGraph

    // constructor / destructor
    template <typename N, typename E>
    ListGraph<N, E>::ListGraph() : _edges(0), _directed(false), _weighed(false) {}

    template <typename N, typename E>
    ListGraph<N, E>::ListGraph(bool directed, bool weighed) : _edges(0), _directed(directed), _weighed(weighed) {}

    template <typename N, typename E>
    ListGraph<N, E>::~ListGraph() {}

    // accessors
    template <typename N, typename E>
    bool ListGraph<N, E>::empty() const { return this->_nodes.empty(); }

    template <typename N, typename E>
    std::size_t ListGraph<N, E>::nodes() const { return this->_nodes.size(); }

    template <typename N, typename E>
    const int& ListGraph<N, E>::edges() const { return this->_edges; }

    template <typename N, typename E>
    const bool& ListGraph<N, E>::directed() const { return this->_directed; }

    template <typename N, typename E>
    const bool& ListGraph<N, E>::weighed() const { return this->_weighed; }

    template <typename N, typename E>
    const std::map<N, std::vector<typename ListGraph<N, E>::Edge>>& ListGraph<N, E>::list() const { return this->_list; }

    template <typename N, typename E>
    bool ListGraph<N, E>::connected() const {

        if (this->_nodes.empty())
            return false;

        if (this->_directed)
            return this->components().size() == 1;

        auto* visited = new bool[this->_nodes.size()]();

        if (!this->_list.empty())
            this->_dfs(this->_list.begin()->first, visited, nullptr);

        for (int i = 0; i < this->_nodes.size(); ++i) {

            if (!visited[i]) {

                delete [] visited;
                return false;

            }

        }

        delete [] visited;
        return true;

    }

    template <typename N, typename E>
    std::vector<std::vector<N>> ListGraph<N, E>::components() const {

        std::vector<std::vector<N>> components;

        if (!this->_directed) {

            std::vector<N> component;
            bool* visited = new bool[this->_nodes.size()]();

            for (const auto& [key, value] : this->_nodes) {

                if (!visited[value]) {

                    component.clear();
                    this->_dfs(key, visited, &component);
                    components.emplace_back(component);

                }

            }

            delete [] visited;

        } else {

            // Tarjan's Strongly Connected Component Algorithm
            int id = 0;
            bool* onStack = new bool[this->_nodes.size()]();
            int* ids = new int[this->_nodes.size()]();
            int* low = new int[this->_nodes.size()]();
            std::stack<N> stack;

            std::fill_n(ids, this->_nodes.size(), -1);

            std::function<void(N)> _dfs = [&](N node) {

                stack.emplace(node);
                onStack[this->_nodes.at(node)] = true;
                ids[this->_nodes.at(node)] = id;
                low[this->_nodes.at(node)] = id++;

                for (const auto& edge : this->_list.at(node)) {

                    if (ids[this->_nodes.at(edge.toNode)] == -1)
                        _dfs(edge.toNode);

                    if (onStack[this->_nodes.at(edge.toNode)])
                        low[this->_nodes.at(node)] = std::min(
                            low[this->_nodes.at(node)],
                            low[this->_nodes.at(edge.toNode)]
                        );

                }

                if (ids[this->_nodes.at(node)] == low[this->_nodes.at(node)]) {

                    components.emplace_back(std::vector<N>(0));

                    for (N n = stack.top(); ; n = stack.top()) {

                        stack.pop();
                        onStack[this->_nodes.at(n)] = false;
                        components.back().emplace_back(n);

                        if (n == node)
                            break;

                    }

                    std::reverse(components.back().begin(), components.back().end());

                }

            };

            for (const auto& [key, value] : this->_nodes)
                if (ids[value] == -1)
                    _dfs(key);

            delete [] onStack;
            delete [] ids;
            delete [] low;

        }

        return components;

    }

    // modifiers
    template <typename N, typename E>
    void ListGraph<N, E>::directed(const bool& directed) { this->_directed = directed; }

    template <typename N, typename E>
    void ListGraph<N, E>::weighed(const bool& weighed) { this->_weighed = weighed; }

    // public methods
    template <typename N, typename E>
    void ListGraph<N, E>::pushNode(const N& value) {

        if (this->_nodes.count(value) == 1) {

            std::cerr << "node already exists" << std::endl;
            return;

        }

        this->_nodes[value] = this->_nodes.size();
        this->_list[value] = std::vector<Edge>(0);

    }

    template <typename N, typename E>
    void ListGraph<N, E>::popNode(const N& value) {

        if (this->_nodes.count(value) == 0) {

            std::cerr << "no such node" << std::endl;
            return;

        }

        int index = this->_nodes[value];

        this->_edges -= this->_list[value].size();
        this->_list.erase(value);

        for (auto& [key, edges] : this->_list) {

            for (int i = 0; i < edges.size(); ++i) {

                if (edges[i].toNode == value) {

                    edges.erase(edges.begin() + i--);

                    if (this->_directed)
                        --this->_edges;

                }

            }

        }

        for (const auto& [key, value] : this->_nodes)
            if (value > index)
                --this->_nodes[key];

        this->_nodes.erase(value);

    }

    template <typename N, typename E>
    void ListGraph<N, E>::pushEdge(const N& from, const N& to, const E& weight) {

        if (this->_nodes.count(from) == 0 || this->_nodes.count(to) == 0) {

            std::cerr << "no such node" << std::endl;
            return;

        }

        this->_list[from].emplace_back(to, weight);

        if (!this->_directed)
            this->_list[to].emplace_back(from, weight);

        ++this->_edges;

    }

    template <typename N, typename E>
    void ListGraph<N, E>::pushEdge(const N& from, const N& to) {

        if (this->_nodes.count(from) == 0 || this->_nodes.count(to) == 0) {

            std::cerr << "no such node" << std::endl;
            return;

        }

        this->_list[from].emplace_back(to, E());

        if (!this->_directed)
            this->_list[to].emplace_back(from, E());

        ++this->_edges;

    }

    template <typename N, typename E>
    void ListGraph<N, E>::popEdge(const N& from, const N& to) {

        if (this->_nodes.count(from) == 0 || this->_nodes.count(to) == 0) {

            std::cerr << "no such node" << std::endl;
            return;

        }

        for (int i = 0; i < this->_list[from].size(); ++i) {

            if (this->_list[from][i].toNode == to) {

                this->_list[from].erase(this->_list[from].begin() + i);
                break;

            }

        }

        if (!this->_directed) {

            for (int i = 0; i < this->_list[to].size(); ++i) {

                if (this->_list[to][i].toNode == from) {

                    this->_list[to].erase(this->_list[to].begin() + i);
                    break;

                }

            }

        }

        --this->_edges;

    }

    template <typename N, typename E>
    void ListGraph<N, E>::popEdges() {

        for (auto& [key, edges] : this->_list)
            edges.clear();

        this->_edges = 0;

    }

    template <typename N, typename E>
    void ListGraph<N, E>::clear() {

        for (auto& [key, edges] : this->_list)
            edges.clear();

        this->_edges = 0;
        this->_list.clear();
        this->_nodes.clear();

    }

    template <typename N, typename E>
    void ListGraph<N, E>::print() const {

        for (const auto& [key, edges] : this->_list) {

            std::cout << this->_nodes.at(key) << " ";

            for (const auto& edge : edges)
                std::cout << "(" << edge.toNode << ", " << edge.value << ") ";
            std::cout << std::endl;

        }

    }

    template <typename N, typename E>
    void ListGraph<N, E>::dfs(const N& node, bool* visited) const { this->_dfs(node, visited, nullptr); }

    template <typename N, typename E>
    void ListGraph<N, E>::dfs(const N& node, std::vector<N>& path) const {

        bool* visited = new bool[this->_nodes.size()];

        this->_dfs(node, visited, &path);
        delete [] visited;

    }

    template <typename N, typename E>
    std::vector<N> ListGraph<N, E>::dfs(const N& node) const {

        std::vector<N> path;

        this->dfs(node, path);
        return path;

    }

    template <typename N, typename E>
    template <typename T>
    void ListGraph<N, E>::bfs(N node, std::map<N, T>& distance, std::function<T(const E&)> f) const {

        bool* visited = new bool[this->_nodes.size()]();
        std::queue<N> queue;

        for (const auto& [key, value] : this->_nodes)
            distance[key];

        visited[this->_nodes.at(node)] = true;
        queue.emplace(node);

        while (!queue.empty()) {

            node = queue.front();
            queue.pop();

            for (const auto& edge : this->_list.at(node))

                if (!visited[this->_nodes.at(edge.toNode)]) {

                    visited[this->_nodes.at(edge.toNode)] = true;
                    queue.emplace(edge.toNode);
                    distance[edge.toNode] = distance[node] + f(edge.value);

                }

        }

        delete [] visited;

    }

    template <typename N, typename E>
    template <typename T>
    std::map<N, T> ListGraph<N, E>::bfs(N node, std::function<T(const E&)> f) const {

        bool* visited = new bool[this->_nodes.size()]();
        std::map<N, T> distance;
        std::queue<N> queue;

        for (const auto& [key, value] : this->_nodes)
            distance[key];

        visited[this->_nodes.at(node)] = true;
        queue.emplace(node);

        while (!queue.empty()) {

            node = queue.front();
            queue.pop();

            for (const auto& edge : this->_list.at(node))

                if (!visited[this->_nodes.at(edge.toNode)]) {

                    visited[this->_nodes.at(edge.toNode)] = true;
                    queue.emplace(edge.toNode);
                    distance[edge.toNode] = distance[node] + f(edge.value);

                }

        }

        delete [] visited;
        return distance;

    }

    template <typename N, typename E>
    template <typename T>
    std::map<N, T*> ListGraph<N, E>::dijkstra(N node, std::function<T(const E&)> f) {

        std::map<N, T*> distance;
        std::priority_queue<std::pair<T, N>> pqueue;

        for (const auto& [key, value] : this->_nodes)
            distance[key] = nullptr;

        distance[node] = new T();
        pqueue.emplace(*distance[node], node);

        while (!pqueue.empty()) {

            node = pqueue.top().second;
            pqueue.pop();

            for (const auto& edge : this->_list[node]) {

                if (!distance[edge.toNode] || *distance[edge.toNode] > *distance[node] + f(edge.value)) {

                    if (!distance[edge.toNode])
                        distance[edge.toNode] = new T();

                    *distance[edge.toNode] = *distance[node] + f(edge.value);
                    pqueue.emplace(*distance[edge.toNode] * -1, edge.toNode);

                }

            }

        }

        return distance;

    }

    // private methods
    template <typename N, typename E>
    void ListGraph<N, E>::_dfs(const N& node, bool* visited, std::vector<N>* path) const {

        visited[this->_nodes.at(node)] = true;

        if (path)
            path->emplace_back(node);

        for (const auto& edge : this->_list.at(node))
            if (!visited[this->_nodes.at(edge.toNode)])
                this->_dfs(edge.toNode, visited, path);

    }

} // namespace ng