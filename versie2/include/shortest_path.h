#ifndef shortest_path_h
#define shortest_path_h

#include "graph.h"
//#include <vector>
//#include <unordered_map>

// Get the length of the shortest path between two nodes
double shortest_path_length(const Graph& graph, VertexID from, VertexID to);

// Get the shortest path
struct PathElement {
  VertexID vertex_id;
  double path_length;
};
typedef std::vector<PathElement> Path;
Path shortest_path(const Graph& graph, VertexID from, VertexID to);

// Get the lengths of all shortest paths from a node
//typedef std::unordered_map<VertexID, double> PathLengths;
typedef std::vector<double> PathLengths;
PathLengths all_shortest_path_lengths(const Graph& graph, VertexID from);

// Get all shortest paths from a node
Graph all_shortest_paths(const Graph& graph, VertexID from);


//// Diameter of the graph: maximum shortest path
//int diameter(const Graph& graph);
//
//// ==== Connected components
//// Defined in connected_components.cpp
//typedef std::unordered_map<VertexID, VertexID> Components;
//Components connected_components(const Graph& graph);


#endif

