#ifndef csv_h
#define csv_h

#include "graph.h"
#include <string>

Graph graph_from_csv(const std::string& vertices_fn, const std::string& edges_fn);
Graph graph_from_csv(const std::string& edges_fn);

void graph_to_csv(const Graph& graph, const std::string& vertices_fn, const std::string& edges_fn);
void graph_to_csv(const Graph& graph, const std::string& edges_fn);

#endif
