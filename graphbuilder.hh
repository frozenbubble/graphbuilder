#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include <string>
#include <utility>

#include "graph.hh"

class GraphBuilder
{
    public:
    virtual Graph* build() = 0;
};

class FileGraphBuilder : public GraphBuilder
{
    private:
    std::string filename;

    public:
    FileGraphBuilder(std::string& f): filename(f){}
    Graph* build() override;
};

class CommandLineGraphBuilder : public GraphBuilder
{
    public:
    Graph* build() override;
};

std::pair<int, int> parseEdge(std::string& line);

#endif