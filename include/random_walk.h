#ifndef random_walk_h
#define random_walk_h

#include "graph.h"

void random_walk(const Graph& graph, vertex_id from, double alpha = 0.85);
void random_walk2(const Graph& graph, vertex_id from, double alpha = 0.85);

#endif
