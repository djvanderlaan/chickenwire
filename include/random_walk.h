#ifndef random_walk_h
#define random_walk_h

#include "graph.h"

// ======== INPUT TYPES
// A number of classes are defined that can be used to pass data to the random walk
// functions. These functions expect the graph and the values of the vertices.
typedef std::vector<double> VertexDoubleValues;
typedef std::vector<VertexType> VertexCategoricalValues;
typedef std::vector<double> VertexWeights;
typedef std::vector<VertexDoubleValues> RandomWalkResult;

// ========== RANDOM WALK FUNCTONS

VertexDoubleValues random_walk_continuous(const Graph& graph, const VertexDoubleValues& vertex_values, 
  double alpha = 0.85, unsigned int nworkers = 0, unsigned int nstep_max = 200, 
  double precision = 1E-5, int* nsteps = 0);

VertexDoubleValues random_walk_continuous(const Graph& graph, const VertexDoubleValues& vertex_values, 
  const VertexWeights& vertex_weights, double alpha = 0.85, unsigned int nworkers = 0, 
  unsigned int nstep_max = 200, double precision = 1E-5, int* nstep = 0);

RandomWalkResult random_walk_categorical(const Graph& graph, 
  const VertexCategoricalValues& vertex_values, 
  double alpha = 0.85, unsigned int nworkers = 0, unsigned int nstep_max = 200, 
  double precision = 1E-5, int* nstep = 0);

RandomWalkResult random_walk_categorical(const Graph& graph, 
  const VertexCategoricalValues& vertex_values, 
  const VertexWeights& vertex_weights, 
  double alpha = 0.85, unsigned int nworkers = 0, unsigned int nstep_max = 200, 
  double precision = 1E-5, int* nstep = 0);

#endif
