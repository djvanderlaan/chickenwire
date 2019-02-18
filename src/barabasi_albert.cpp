#include "functions.h"

#include <cmath>
#include <cstdlib>
#include <map>

inline double runif() {
  return (double)std::rand() / (double)RAND_MAX;
}

Graph& barabasi_albert(Graph& graph, VertexList::size_type size) {
  //const unsigned int blocksize = 2000;
  unsigned int blocksize = std::max(std::round(std::sqrt((double)size/1E6*2.0)), 1.0) * 1024;
  //unsigned int blocksize = std::max(500.0, 2.0*std::sqrt((double)size));
  std::map<VertexList::size_type, double> blocks;
  blocks[0] = 0.0;

  for (VertexList::size_type i = 0; i < size; ++i) {
    graph.add_vertex(i);
    double s = (2.0*i+1.0)*runif();

    // Look up block to start with
    VertexList::size_type start = 0;
    double sum = 0.0;
    for (auto p = blocks.begin(); p != blocks.end(); ++p) {
      if (p->second < s) {
        start = p->first;
        sum = p->second;
      } else break;
    }
    // Find selected node
    for (VertexList::size_type j = start; j <= i; ++j) {
      const Vertex& v = graph.vertex(j);
      EdgeCount k = 1; // TODO v.in_degree();
      if (v.connected_to(j)) ++k; // self loops are only counted once

      sum += k == 0 ? 1.0 : k;
      if (sum > s) {
        graph.add_edge(i, j);
        // Update blocks
        for (VertexList::size_type k = start; k <= i; k += blocksize) {
          if (k == i) blocks[k] = 2.0 * i;
          if (k > j) blocks[k] += 1.0;
        }
        // And ... next
        break;
      }
    }
  }
  return graph;
}


/* 
// Old simpler version
Graph& barabasi_albert(Graph& graph, VertexList::size_type size) {
  for (VertexList::size_type i = 0; i < size; ++i) {
    graph.add_vertex(i);
    double s = (2.0*i+1.0)*runif();
    double sum = 0.0;
    for (VertexList::size_type j = 0; j <= i; ++j) {
      const Vertex& v = graph.vertex(j);
      const EdgeCount k = v.in_degree() + 1;
      sum += k;
      if (sum > s) {
        graph.add_edge(i, j);
        break;
      }
    }
  }
  return graph;
}
*/
