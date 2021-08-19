#include <iostream>
#include <string>
#include <utility>

#include "graphbuilder.hh"

int main() {
    std::cout << "hello from manual test." << std::endl;

    CommandLineGraphBuilder builder;
    Graph* g = builder.build();

    std::cout << std::endl << "Edges" << std::endl;
    auto dependencies = g->getDependencies();
    for (auto&& edge : *dependencies)
    {
        std::cout << edge.first << "-->" << edge.second << std::endl;
    }

    std::cout << std::endl << "Source nodes" << std::endl;
    auto sourceNodes = g->getSourceNodes();
    for (int sourceNode : *sourceNodes)
    {
        std::cout << sourceNode << std::endl;
    }

    std::cout << std::endl << "DAG: " << g->isDag() << std::endl;

    std::cout << std::endl << "Batches: " << std::endl;
    auto batches = g->getParallelBatches();
    for (auto&& batch : *batches)
    {
        std::cout << "Batch: ";
        for (int node : batch)
        {
            std::cout << node << " ";
        }
        std::cout << std::endl;
        
    }
    
    delete dependencies;
    delete sourceNodes;
    delete batches;
    delete g;
}