#ifndef GRAPH_H
#define GRAPH_H

#include <exception>
#include <set>
#include <string>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 * Represents a node in a dependency graph
 */
class Node
{
    private:
    int number;
    std::set<int> inputs;
    std::set<int> outputs;
    
    public:
    
    /**
     * Default constructor.
     */
    Node() {}    
    
    /**
     * Constructor to initialize node with id.
     *
     * @param nodeNumber the id of this node
     */
    Node(int nodeNumber): number(nodeNumber) {}
    
    /**
     * Gets the node id.
     *
     * @return the node id.
     */
    int getNumber() { return number; }
    
    /**
     * Gets the ids of nodes that this node uses as inputs.
     *
     * @return the list of input ids
     */
    const std::set<int>& getInputs() { return inputs; }
    
    /**
     * Gets the ids of nodes that this node uses as outputs.
     *
     * @return the list of output ids
     */
    const std::set<int>& getOutputs() { return outputs; }
    
    /**
     * Adds an input to this node.
     *
     * @param i id of the input node
     */
    void addInput(int i) { inputs.insert(i); }
    
    /**
     * Adds an output to this node.
     *
     * @param i id of the output node
     */
    void addOutput(int o) { outputs.insert(o); }

};

/**
 * Represents a dependency graph
 */
class Graph
{
    private:
    std::vector<Node> nodes;

    public:
    Graph(int nodeCount);

    /**
     * Nodes that make up the graph.
     *
     * @return the nodes in this graph
     */
    const std::vector<Node>& getNodes() { return nodes; }

    /**
     * Adds an X-->Y edge to the graph meaning X's output is required by Y.
     *
     * @param source source node
     * @param destination destination node
     */
    void addEdge(int source, int destination);

    /**
     * Adds multiple edge to the graph.
     *
     * @param edges list of edges as {source_id, destination_id} pairs
     */
    void addEdges(const std::vector<std::pair<int, int>>& edges);

    /**
     * Returns the ids of all nodes without inputs.
     *
     * @return the source node ids in the dependency graph
     */
    const std::vector<int> getSourceNodes();

    /**
     * Returns the edges in this dependency graph.
     *
     * @return the edges of the dependency graph in {source_id, destination_id} format
     */
    const std::vector<std::pair<int, int>> getDependencies();

    /**
     * Returns the edges in this dependency graph.
     * Starts a BFS from all nodes without inputs. In each cycle it tests whether the in progress nodes
     * (nodes that are visited but not all dependencies are satisfied) can be finished (all dependencies are satisfied)
     * and if so puts them in a batch and adds the batch to the list of batches.
     *
     * @return the edges of the dependency graph in {source_id, destination_id} format
     */
    const std::vector<std::set<int>> getParallelBatches();

    /**
     * Checks whether this graph contains circular dependencies.
     * Launches a DFS from unvisited nodes until all nodes are visited.
     * Stores the ids to be visited on a stack. If an id appeares twice on the stack it returns false.
     *
     * @return true if the graph contains no circular dependencies
     */
    bool isDag();
};

class GraphException : public std::logic_error
{
    public :
    GraphException(const char* msg): std::logic_error(msg) {}
};

#endif