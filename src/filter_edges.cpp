#include "filter_edges.h"
#include <algorithm>

class is_of_type {
  public:
    is_of_type(edge_type type): type_(type) {}

    bool operator()(const Edge& edge) {
      return edge.type() == type_;
    }

  private:
    edge_type type_;
};

void filter_edges_on_type(Graph& graph, edge_type type) {
  VertexList& vertices = graph.vertices_nonconst();
  for (auto vertexp = vertices.begin(); vertexp != vertices.end(); ++vertexp) {
    EdgeList& edges_out = vertexp->second.edges_out_nonconst();
    edges_out.erase(std::remove_if(edges_out.begin(), edges_out.end(), 
      is_of_type(type)), edges_out.end());
    EdgeList& edges_in = vertexp->second.edges_in_nonconst();
    edges_in.erase(std::remove_if(edges_in.begin(), edges_in.end(), 
      is_of_type(type)), edges_in.end());
  }
}

void reweigh_edges_by_vertex(Graph& graph, edge_weight scale) {
  VertexList& vertices = graph.vertices_nonconst();
  for (auto vertexp = vertices.begin(); vertexp != vertices.end(); ++vertexp) {
    EdgeList& edges_out = vertexp->second.edges_out_nonconst();

    // Calculate sum
    float sum = 0.0;
    for (auto e = edges_out.begin(); e != edges_out.end(); ++e) {
      sum += e->weight();
    }
    if (sum == 0.0) sum = 1.0;
    // Reweigh
    for (auto e = edges_out.begin(); e != edges_out.end(); ++e) {
      e->weight(e->weight()/sum*scale);
    }
    //TODO also update incoming weights????
  }
}

void reweigh_edges_by_vertex_and_type(Graph& graph, edge_weight scale) {
  VertexList& vertices = graph.vertices_nonconst();
  for (auto vertexp = vertices.begin(); vertexp != vertices.end(); ++vertexp) {
    EdgeList& edges_out = vertexp->second.edges_out_nonconst();

    // Calculate sum
    std::unordered_map<edge_type, float> sums;
    for (auto e = edges_out.begin(); e != edges_out.end(); ++e) {
      sums[e->type()] = sums[e->type()] + e->weight();
    }
    edge_weight ntypes = sums.size();
    // Reweigh
    for (auto e = edges_out.begin(); e != edges_out.end(); ++e) {
      if (sums[e->type()] == 0) sums[e->type()] = 1.0;
      e->weight(e->weight()/sums[e->type()]*scale/ntypes);
    }
    //TODO also update incoming weights????
  }
}

