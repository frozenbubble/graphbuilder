# graphbuilder

## Build

In the source directory you can run

    cmake -S . -B build
    cmake --build build

To run tests, from the build directory run

    ctest

Or alternatively if you are on linux, you can simply run

    ./buildtest.sh

which will build and run the tests as well. Binaries will be under the 'build' folder.

## Usage

To construct a graph you can use the CommandLineGraphBuilder or the FileGraphBuilder or you can subclass the GraphBuilder class. Alternatively you can create a Graph initializing it with the number of nodes it should hold and adding the edges one by one using Graph::addEdge or Graph::addEdges. To query the nodes that can run in parallel call Graph::getParallelBatches. See header files for documentation or *manualtest.cc* for examples.

## Further improvements

* Generate large graphs for testing
* Configure cpplint
* Implement move semantics for Graph