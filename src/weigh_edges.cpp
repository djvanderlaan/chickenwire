#include "weigh_edges.h"
#include <unordered_map>

void reweigh_edges_by_vertex(Graph& graph, EdgeWeight scale) {
  VertexList& vertices = graph.vertices_nonconst();
  for (auto vertexp = vertices.begin(); vertexp != vertices.end(); ++vertexp) {
    EdgeList& edges = vertexp->edges_nonconst();

    // Calculate sum
    EdgeWeight sum = 0.0;
    for (auto e = edges.begin(); e != edges.end(); ++e) {
      sum += e->weight();
    }
    if (sum == 0.0) sum = 1.0;
    // Reweigh
    for (auto e = edges.begin(); e != edges.end(); ++e) {
      e->weight(e->weight()/sum*scale);
    }
  }
}

void reweigh_edges_by_vertex_and_layer(Graph& graph, EdgeWeight scale) {
  VertexList& vertices = graph.vertices_nonconst();
  for (auto vertexp = vertices.begin(); vertexp != vertices.end(); ++vertexp) {
    EdgeList& edges = vertexp->edges_nonconst();

    // Calculate sum
    std::unordered_map<EdgeType, float> sums;
    for (auto e = edges.begin(); e != edges.end(); ++e) {
      sums[e->type()] = sums[e->type()] + e->weight();
    }
    EdgeWeight ntypes = sums.size();
    // Reweigh
    for (auto e = edges.begin(); e != edges.end(); ++e) {
      if (sums[e->type()] == 0) sums[e->type()] = 1.0;
      e->weight(e->weight()/sums[e->type()]*scale/ntypes);
    }
  }
}


