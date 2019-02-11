#ifndef functions_h
#define functions_h

#include "graph.h"

// ==== Random networks
// Defined in barabasi_albert.cpp
Graph& barabasi_albert(Graph& graph, GraphSize size);

// ==== Reading writing
// Defined in csv.cpp
//#include <istream>
//Graph read_from_csv(std::istream& input);
//Graph read_from_csv(const std::string& filename);
//void write_to_csv(const Graph& graph, std::ostream& output);
//void write_to_csv(const Graph& graph, const std::string& filename);

// ==== Shortest path
// Defined in dijkstra.cpp
//double shortest_path(const Graph& graph, vertex_id from, vertex_id to);
//int diameter(const Graph& graph);

// ==== Connected components
// Defined in connected_components.cpp
typedef std::unordered_map<vertex_id, vertex_id> Components;
Components connected_components(const Graph& graph);


// ==== Degree distribution
// Defined in degree_distribution.cpp
//#include <map>
//typedef std::map<EdgeCount, unsigned int> DegreeDistribution;
//typedef std::map<std::pair<EdgeCount, EdgeCount>, unsigned int> DegreeMatrix;
//
//DegreeDistribution in_degree_distribution(const Graph& graph);
//DegreeDistribution out_degree_distribution(const Graph& graph);
//DegreeMatrix degree_distribution_matrix(const Graph& graph);
//double mean_in_degree(const Graph& graph);
//double mean_out_degree(const Graph& graph);

#endif

