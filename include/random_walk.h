#ifndef random_walk_h
#define random_walk_h

#include "graph.h"

void random_walk(const Graph& graph, VertexID from, double alpha = 0.85);
void random_walk2(const Graph& graph, VertexID from, double alpha = 0.85);
void random_walk_rev(const Graph& graph, VertexID from, double alpha = 0.85);

typedef std::unordered_map<VertexID, double> VertexDoubleValues;
void random_walk_cont(const Graph& graph, const VertexDoubleValues& vertex_values, double alpha = 0.85);

#endif
