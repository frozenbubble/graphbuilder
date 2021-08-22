#include "graph.hh"

#include <stack>
#include <stdexcept>

Graph::Graph(int nodeCount): nodes(nodeCount)
{
    if (nodeCount == 0)
    {
        throw std::invalid_argument("Graph cannot be empty");
    }
    
    for (int i = 0; i < nodes.size(); i++)
    {
        nodes[i] = Node(i);
    }
}

void Graph::addEdge(int source, int destination)
{
    int maxNode = nodes.size() - 1;
    if (source > maxNode || destination > maxNode)
    {
        std::string message = "source and destination ids have to be between" + std::to_string(0) + " and " + std::to_string(maxNode);
        throw std::out_of_range(message);
    }

    auto&& destinationOutputs = nodes[destination].getOutputs();
    if (source == destination || destinationOutputs.find(source) != destinationOutputs.end())
    {
        throw std::invalid_argument("Back edges are not allowed.");
    }
    

    Node& s = nodes[source];
    Node& d = nodes[destination];

    s.addOutput(destination);
    d.addInput(source);
}

void Graph::addEdges(const std::vector<std::pair<int, int>>& edges)
{
    for (auto&& edge : edges)
    {
        addEdge(edge.first, edge.second);
    }
    
}

const std::vector<std::pair<int, int>> Graph::getDependencies()
{
    std::vector<std::pair<int, int>> dependencies;
    
    for (auto&& node : nodes)
    {
        for(int input : node.getOutputs())
        {
            dependencies.push_back({node.getNumber(), input});
        }
    }

    return dependencies;
}

const std::vector<std::set<int>> Graph::getParallelBatches()
{
    if (!isDag())
    {
        throw GraphException("Graph contains circular dependency.");
    }

    std::vector<std::set<int>> parallelBatches;
    auto sourceNodes = getSourceNodes();
    std::set<int> inProgress;
    std::set<int> done;

    std::copy(sourceNodes.begin(), sourceNodes.end(), std::inserter(inProgress, inProgress.end()));
    while (done.size() != nodes.size())
    {
        //add every node with satisfied dependencies to the current batch
        std::set<int> currentBatch;
        for (int currentNode : inProgress)
        {
            //check if all dependencies are done
            bool allDependenciesDone = true;
            for (int d : nodes[currentNode].getInputs())
            {
                if (done.find(d) == done.end())
                {
                    allDependenciesDone = false;
                }
            }

            if (allDependenciesDone)
            {
                currentBatch.insert(currentNode);
            }
        }

        //add nodes finished in the current batch to the done list.
        for (int node : currentBatch)
        {
            inProgress.erase(node);
            done.insert(node);
            for (int output : nodes[node].getOutputs())
            {
                inProgress.insert(output);
            }
        }

        parallelBatches.push_back(currentBatch);
    }
    
    return parallelBatches;
}

const std::vector<int> Graph::getSourceNodes()
{
    //find all nodes without inputs
    std::vector<int> sourceNodes;
    for (auto&& node : nodes)
    {
        if (node.getInputs().empty()) 
        {
            sourceNodes.push_back(node.getNumber());
        }
    }

    return sourceNodes;
}

bool Graph::isDag() 
{
    std::set<int> todo;
    for (int i = 0; i < nodes.size(); i++)
    {
        todo.insert(i);
    }

    std::set<int> visited;
    std::stack<int> stack;
    std::set<int> stackContents;

    //if there are multiple source nodes, take one from the not done list 
    //and start DFS from that node
    while(todo.size() > 0)
    {
        int root = *todo.begin();
        stack.push(root);
        stackContents.insert(root);

        while(!stack.empty())
        {
            auto&& current = nodes[stack.top()];
            int currentNum = current.getNumber();
            auto&& currentOutputs = current.getOutputs();

            //if not visited and has outputs, push outputs to the stack
            if (currentOutputs.size() > 0 && visited.find(currentNum) == visited.end())
            {
                for (int output : currentOutputs)
                {
                    if(stackContents.find(output) != stackContents.end())
                    {
                        return false;
                    }

                    stack.push(output);
                    stackContents.insert(output);
                }

                //tag nodes whose outputs are pushed to the stack
                visited.insert(currentNum);
            }
            //if it is visited and is on top of the stack
            //we're done here
            else
            {
                stack.pop();
                stackContents.erase(currentNum);
                todo.erase(currentNum);
            }
        }
    }

    return true;
}