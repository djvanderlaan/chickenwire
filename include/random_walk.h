#ifndef random_walk_h
#define random_walk_h

#include "graph.h"
#include <unordered_map>

void random_walk(const Graph& graph, VertexID from, double alpha = 0.85);
void random_walk2(const Graph& graph, VertexID from, double alpha = 0.85);
void random_walk_rev(const Graph& graph, VertexID from, double alpha = 0.85);

typedef std::unordered_map<VertexID, double> VertexDoubleValues;
void random_walk_cont(const Graph& graph, const VertexDoubleValues& vertex_values, double alpha = 0.85);
void random_walk_cont2(const Graph& graph, const VertexDoubleValues& vertex_values, double alpha = 0.85);

// Perhaps VertexCategoricalValues is not needed as type is also stored in the
// vertices. Or, perhaps type is not needed in the vertices
typedef std::unordered_map<VertexID, VertexType> VertexCategoricalValues;
void random_walk_cat(const Graph& graph, const VertexCategoricalValues& vertex_values, double alpha = 0.85);


template<typename T, typename S> 
std::unordered_map<VertexID, T> initialise_rw_data(const Graph& graph, const S& vertex_values) {
  std::unordered_map<VertexID, T> values;
  for (auto p = vertex_values.begin(); p != vertex_values.end(); ++p) {
    values[p->first] = T(p->second);
  }
  return values;
}

template<typename T, typename S>
void random_walk_template(const Graph& graph, const S& vertex_values, double alpha) { 
  // Initialise values
  std::unordered_map<VertexID, T> values = initialise_rw_data<T>(graph, vertex_values);
  // Initialise loop
  unsigned int max_step = 50;
  const VertexList& vertices = graph.vertices();
  bool cont = true;
  // Start iterating
  for (unsigned int step = 0; step < max_step && cont; ++step) {
    // Loop over all edges
    for (auto vert = vertices.cbegin(); vert != vertices.cend(); ++vert) {
      T& value = values[vert->second.id()];
      value.start_update();
      const EdgeList& edges = vert->second.edges();
      for (auto e = edges.cbegin(); e != edges.cend(); ++e) {
        const T& target_value = values[e->dst()];
        value.update(target_value, e->weight(), alpha);
      }
    }
    // Signal next run and check if we need to stop
    cont = false;
    for (auto p = values.begin(); p != values.end(); ++p) cont |= p->second.end_update();
  }
}



#endif
