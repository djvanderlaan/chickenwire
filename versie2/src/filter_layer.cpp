#include "filter_layer.h"
#include <algorithm>

class edge_is_in_layer {
  public:
    edge_is_in_layer(EdgeType type): type_(type) {}

    bool operator()(const Edge& edge) {
      return edge.type() == type_;
    }

  private:
    EdgeType type_;
};

void remove_layer(Graph& graph, EdgeType layer) {
  VertexList& vertices = graph.vertices_nonconst();
  for (auto vertexp = vertices.begin(); vertexp != vertices.end(); ++vertexp) {
    EdgeList& edges = vertexp->edges_nonconst();
    edges.erase(std::remove_if(edges.begin(), edges.end(), 
      edge_is_in_layer(layer)), edges.end());
  }
}

