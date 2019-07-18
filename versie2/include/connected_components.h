#ifndef connected_components_h
#define connected_components_h

#include "graph.h"
#include <vector>

std::vector<VertexID> connected_components(const Graph& graph);
size_t nconnected_components(const Graph& graph);

#endif

