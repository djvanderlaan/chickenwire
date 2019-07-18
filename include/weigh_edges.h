#ifndef weigh_edges_h
#define weigh_edges_h

#include "graph.h"

// Make sure that all outgoing edge weights of a vertex sum to `scale`
void reweigh_edges_by_vertex(Graph& graph, EdgeWeight scale = 1.0);

// Make sure that all outgoing edge weights of a vertex sum to `scale`
// Also make sure that for each layer in which a vertex is present, 
// the outgoing weightd sum to the same value. For example, for 
// vertex A below, the weights on each layer sum to 0.5. The sum of
// all weights is 1.0.
//
// A->B layer: X -> weight = 0.25
// A->C layer: X -> weight = 0.25
// A->D layer: Y -> weight = 0.50
//
void reweigh_edges_by_vertex_and_layer(Graph& graph, EdgeWeight scale = 1.0);

#endif
