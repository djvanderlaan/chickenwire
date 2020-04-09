#ifndef degree_distribution_h
#define degree_distribution_h

#include "graph.h"
#include <map>

typedef std::map<EdgeCount, unsigned int> DegreeDistribution;
typedef std::map<std::pair<EdgeCount, EdgeCount>, unsigned int> DegreeMatrix;

DegreeDistribution in_degree_distribution(const Graph& graph);
DegreeDistribution out_degree_distribution(const Graph& graph);
DegreeMatrix degree_distribution_matrix(const Graph& graph);
double mean_in_degree(const Graph& graph);
double mean_out_degree(const Graph& graph);

#endif
