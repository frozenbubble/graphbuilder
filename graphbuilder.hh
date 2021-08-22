#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include <string>
#include <utility>

#include "graph.hh"

/**
 * Interface for graph builder.
 */
class GraphBuilder
{
    public:
    virtual Graph* build() = 0;
};

/**
 * Graph builder that builds a dependency graph from file.
 */
class FileGraphBuilder : public GraphBuilder
{
    private:
    std::string filename;

    public:
    FileGraphBuilder(std::string& f): filename(f){}
    Graph* build() override;
};

/**
 * Graph builder that builds a dependency graph from command line input.
 */
class CommandLineGraphBuilder : public GraphBuilder
{
    public:
    Graph* build() override;
};

/**
 * Parses a line for a graph edge. Expected format SOURCE,DESTINATION
 * Throws exception if format does not match expected.
 * 
 * @param line line to parse
 * @return pair of edge ids in {SOURCE_ID, DESTINATION_ID} format
 */
std::pair<int, int> parseEdge(std::string& line);

#endif