#include "graphbuilder.hh"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

Graph* FileGraphBuilder::build() {
    std::ifstream input(filename);
    std::string line;
    Graph* graph;

    if (!input.is_open())
    {
        //TODO
    }
    
    if (getline(input, line)) 
    {
        int nodeCount = stoi(line);
        graph = new Graph(nodeCount);      
    }

    std::vector<int> edge;
    while (getline(input, line))
    {
        auto edge = parseEdge(line);
        int source = std::get<0>(edge);
        int destination = std::get<1>(edge);
        graph->addEdge(source, destination);
    }

    return graph;
}

Graph* CommandLineGraphBuilder::build()
{
    int nodeCount;
    std::cout << "Number of nodes: ";
    std::cin >> nodeCount;
    Graph* graph = new Graph(nodeCount);

    std::cout << "Add edges to the graph in X,Y format one by one where X,Y represents an X-->Y edge " << std::endl;
    std::cout << "meaning X's output is required by Y. Hit enter when done" << std::endl;
    std::cin.ignore();

    std::string line;
    while (std::getline(std::cin, line) && line.size() > 0)
    {
        auto edge = parseEdge(line);
        int source = std::get<0>(edge);
        int destination = std::get<1>(edge);
        graph->addEdge(source, destination);
    }

    return graph;
}

std::pair<int,int> parseEdge(std::string& line)
{
    std::vector<int> edge;
    std::stringstream ss(line);

    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        edge.push_back(stoi(substr));
    }

    if (edge.size() != 2)
    {
        throw std::invalid_argument("Invalid format.");
    }
    
    return {edge[0], edge[1]};
}