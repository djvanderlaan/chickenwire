#ifndef graph_store_h
#define graph_store_h

#include "graph.h"

Graph* get_graph(int id);
int register_graph(Graph* graph);
void delete_graph(int id);

#endif
