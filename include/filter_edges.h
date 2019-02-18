#ifndef filter_edges_h
#define filter_edges_h

#include "graph.h"

void filter_edges_on_type(Graph& graph, Edge::type_type type);
void reweigh_edges_by_vertex(Graph& graph, Edge::weight_type scale = 1.0);
void reweigh_edges_by_vertex_and_type(Graph& graph, Edge::weight_type scale = 1.0);

#endif

