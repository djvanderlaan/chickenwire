#ifndef random_walk_h
#define random_walk_h

#include "graph.h"
#include <unordered_map>

typedef std::unordered_map<VertexID, double> VertexDoubleValues;
typedef std::unordered_map<VertexID, VertexType> VertexCategoricalValues;

void random_walk_continuous(const Graph& graph, const VertexDoubleValues& vertex_values, 
  double alpha = 0.85, unsigned int nworkers = 0);
void random_walk_categorical(const Graph& graph, const VertexCategoricalValues& vertex_values, 
  double alpha = 0.85, unsigned int nworkers = 0);


// === Weighted versions

struct WDouble {
   double value;
   double weight;
};

struct WCategorical {
  VertexType value;
  double weight;
};

typedef std::unordered_map<VertexID, WDouble> VertexWDoubleValues;
typedef std::unordered_map<VertexID, WCategorical> VertexWCategoricalValues;

void random_walk_continuous(const Graph& graph, const VertexWDoubleValues& vertex_values, 
  double alpha = 0.85, unsigned int nworkers = 0);
void random_walk_categorical(const Graph& graph, const VertexWCategoricalValues& vertex_values, 
  double alpha = 0.85, unsigned int nworkers = 0);

#endif
