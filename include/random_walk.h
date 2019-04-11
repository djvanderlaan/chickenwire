#ifndef random_walk_h
#define random_walk_h

#include "graph.h"
#include <unordered_map>

typedef std::unordered_map<VertexID, double> VertexDoubleValues;
typedef std::unordered_map<VertexID, VertexType> VertexCategoricalValues;

void random_walk_threaded_continuous(const Graph& graph, const VertexDoubleValues& vertex_values, 
  double alpha = 0.85);
void random_walk_threaded_categorical(const Graph& graph, const VertexCategoricalValues& vertex_values, 
  double alpha = 0.85);

#endif
