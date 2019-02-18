#include "filter_edges.h"
#include <algorithm>

class edge_is_of_type {
  public:
    edge_is_of_type(Edge::type_type type): type_(type) {}

    bool operator()(const Edge& edge) {
      return edge.type() == type_;
    }

  private:
    Edge::type_type type_;
};

void filter_edges_on_type(Graph& graph, Edge::type_type type) {
  VertexList& vertices = graph.vertices_nonconst();
  for (auto vertexp = vertices.begin(); vertexp != vertices.end(); ++vertexp) {
    EdgeList& edges = vertexp->second.edges_nonconst();
    edges.erase(std::remove_if(edges.begin(), edges.end(), 
      edge_is_of_type(type)), edges.end());
  }
}

void reweigh_edges_by_vertex(Graph& graph, Edge::weight_type scale) {
  VertexList& vertices = graph.vertices_nonconst();
  for (auto vertexp = vertices.begin(); vertexp != vertices.end(); ++vertexp) {
    EdgeList& edges = vertexp->second.edges_nonconst();

    // Calculate sum
    float sum = 0.0;
    for (auto e = edges.begin(); e != edges.end(); ++e) {
      sum += e->weight();
    }
    if (sum == 0.0) sum = 1.0;
    // Reweigh
    for (auto e = edges.begin(); e != edges.end(); ++e) {
      e->weight(e->weight()/sum*scale);
    }
    //TODO also update incoming weights????
  }
}

void reweigh_edges_by_vertex_and_type(Graph& graph, Edge::weight_type scale) {
  VertexList& vertices = graph.vertices_nonconst();
  for (auto vertexp = vertices.begin(); vertexp != vertices.end(); ++vertexp) {
    EdgeList& edges = vertexp->second.edges_nonconst();

    // Calculate sum
    std::unordered_map<Edge::type_type, float> sums;
    for (auto e = edges.begin(); e != edges.end(); ++e) {
      sums[e->type()] = sums[e->type()] + e->weight();
    }
    Edge::weight_type ntypes = sums.size();
    // Reweigh
    for (auto e = edges.begin(); e != edges.end(); ++e) {
      if (sums[e->type()] == 0) sums[e->type()] = 1.0;
      e->weight(e->weight()/sums[e->type()]*scale/ntypes);
    }
    //TODO also update incoming weights????
  }
}

