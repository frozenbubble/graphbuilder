#include <gtest/gtest.h>
#include <stdexcept>

#include "graph.hh"

TEST(GraphConstruction, AddForwardEdge) 
{
  Graph g(2);
  auto edges = g.getDependencies();
  EXPECT_EQ(0, edges->size());
  delete edges;

  g.addEdge(0,1);
  edges = g.getDependencies();
  EXPECT_EQ(1, edges->size());
  delete edges;
}

TEST(GraphConstruction, AddBackEdgeSelf) 
{
  Graph g(2);
  EXPECT_THROW(g.addEdge(0,0), std::invalid_argument);
}

TEST(GraphConstruction, AddBackEdgeParent) 
{
  Graph g(2);
  g.addEdge(0,1);
  EXPECT_THROW(g.addEdge(1,0), std::invalid_argument);
}

TEST(GraphConstruction, AddEdgeNodeIndexOutOfBound) 
{
  Graph g(2);
  g.addEdge(0,1);
  EXPECT_THROW(g.addEdge(0,2), std::invalid_argument);
}

TEST(DAGDetection, NotDAG) {
  Graph g(3);
  g.addEdges({{0,1}, {1,2}, {2,0}});
  EXPECT_FALSE(g.isDag());
}

TEST(DAGDetection, DAG) {
  Graph g(10);
  g.addEdges({
    {0,3}, {3,4}, {3,5}, {4,6}, {5,6}, {6,7}, {6,8}, {1,7}, {2,9}, {0,8}
  });
  EXPECT_TRUE(g.isDag());
}

TEST(ParallelBatches, EmptyGraph) {
  EXPECT_THROW(Graph g(0), std::invalid_argument);
}

TEST(ParallelBatches, NoEdges) {
  Graph g(10);
  auto batches = g.getParallelBatches();
  EXPECT_EQ(1, batches->size());
  auto batch = (*batches)[0];
  EXPECT_EQ(10, batch.size());

  delete batches;
}

TEST(ParallelBatches, MultipleSources) {
  Graph g(10);
  g.addEdges({
    {0,3}, {3,4}, {3,5}, {4,6}, {5,6}, {6,7}, {6,8}, {1,7}, {2,9}, {0,8}
  });
  auto batches = g.getParallelBatches();
  EXPECT_EQ(5, batches->size());
  
  //Batch 1
  auto batch1 = (*batches)[0];
  std::set<int> expected1({0, 1, 2});
  EXPECT_EQ(expected1, batch1);

  //Batch 2
  auto batch2 = (*batches)[1];
  std::set<int> expected2({3, 9});
  EXPECT_EQ(expected2, batch2);

  //Batch 3
  auto batch3 = (*batches)[2];
  std::set<int> expected3({4, 5});
  EXPECT_EQ(expected3, batch3);

  //Batch 4
  auto batch4 = (*batches)[3];
  std::set<int> expected4({6});
  EXPECT_EQ(expected4, batch4);

  //Batch 5
  auto batch5 = (*batches)[4];
  std::set<int> expected5({7, 8});
  EXPECT_EQ(expected5, batch5);

  delete batches;
}

TEST(ParallelBatches, NotDAG) {
  Graph g(10);
  g.addEdges({
    {0,3}, {3,4}, {3,5}, {4,6}, {5,6}, {6,7}, {6,8}, {1,7}, {2,9}, {0,8}, {6,3}
  });
  EXPECT_THROW(g.getParallelBatches(), GraphException);
}