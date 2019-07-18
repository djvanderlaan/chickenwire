#include "connected_components.h"
#include <unordered_set>

std::vector<VertexID> connected_components(const Graph& graph) {
  const VertexList& vertices = graph.vertices();

  // Initialise components: put each vertex in its own component, labelled 
  // conveniently equal to the vertexid
  std::vector<VertexID> components(vertices.size());
  for (VertexID i = 0; i < vertices.size(); ++i) components[i] = i;

  bool iterate = true;
  while (iterate) {
    iterate = false;
    VertexID i = 0;
    for (auto p = vertices.cbegin(); p != vertices.cend(); ++p, ++i) {
      const EdgeList& edges = p->edges();
      VertexID id_src = components[i];
      for (auto q = edges.cbegin(); q != edges.cend(); ++q) {
        VertexID id_dst = components[q->dst()];
        if (id_dst > id_src) {
          components[q->dst()] = id_src;
          iterate = true;
        } else if (id_dst < id_src) {
          components[i] = id_dst;
          iterate = true;
        }
      }
    }
  }
  return components;
}

size_t nconnected_components(const Graph& graph) {
  std::vector<VertexID> components = connected_components(graph);
  std::unordered_set<VertexID> unique_components;
  for (auto p = components.cbegin(); p != components.cend(); ++p) 
    unique_components.insert(*p);
  return unique_components.size();
}

