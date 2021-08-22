#ifndef GRAPH_H
#define GRAPH_H

#include <exception>
#include <set>
#include <string>
#include <stdexcept>
#include <utility>
#include <vector>

class Node
{
    private:
    int number;
    std::set<int> inputs;
    std::set<int> outputs;
    
    public:
    Node() {}
    Node(int nodeNumber): number(nodeNumber) {}
    int getNumber() { return number; }
    const std::set<int>& getInputs() { return inputs; }
    const std::set<int>& getOutputs() { return outputs; }
    void addInput(int i) { inputs.insert(i); }
    void addOutput(int o) { outputs.insert(o); }

};

class Graph
{
    private:
    std::vector<Node> nodes;

    public:
    Graph(int nodeCount);
    const std::vector<Node>& getNodes() { return nodes; }
    void addEdge(int source, int destination);
    void addEdges(const std::vector<std::pair<int, int>>& edges);
    const std::vector<int> getSourceNodes();
    const std::vector<std::pair<int, int>> getDependencies();
    const std::vector<std::set<int>> getParallelBatches();
    bool isDag();
};

class GraphException : public std::logic_error
{
    public :
    GraphException(const char* msg): std::logic_error(msg) {}
};


#endif