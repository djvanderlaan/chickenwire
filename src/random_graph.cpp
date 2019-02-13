#include "graph.h"

#include <cmath>
#include <cstdlib>
#include <map>

inline double runif() {
  return (double)std::rand() / (double)RAND_MAX;
}

Graph& random_graph(Graph& graph, VertexList::size_type size, double prob) {
  // Add vertices
  for (VertexList::size_type i = 0; i < size; ++i) {
    graph.add_vertex(i);
  }
  // Add edges
  for (VertexList::size_type src = 0; src < size; ++src) {
    for (VertexList::size_type dst = 0; dst < size; ++dst) {
      if (runif() < prob) graph.add_edge(src, dst);
    }
  }
  return graph;
}
