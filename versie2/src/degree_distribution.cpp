#include "degree_distribution.h"
//#include <utility>

inline std::vector<EdgeCount> calc_in_degree(const Graph& graph) {
  const VertexList& vertices = graph.vertices();
  std::vector<EdgeCount> in_degree(vertices.size());
  for (auto vertp = vertices.begin(); vertp != vertices.end(); ++vertp) {
    const EdgeList& edges = vertp->edges();
    for (auto edgep = edges.begin(); edgep != edges.end(); ++edgep) {
      in_degree[edgep->dst()] += 1;
    }
  }
  return in_degree;
}

DegreeDistribution in_degree_distribution(const Graph& graph) {
  std::vector<EdgeCount> in_degree = calc_in_degree(graph);
  DegreeDistribution counts;
  for (auto p = in_degree.begin(); p != in_degree.end(); ++p) {
    EdgeCount degree = *p;
    counts[degree] = counts[degree] + 1;
  }
  return counts;
}

DegreeDistribution out_degree_distribution(const Graph& graph) {
  DegreeDistribution counts;
  const VertexList& v = graph.vertices();
  for (auto p = v.begin(); p != v.end(); ++p) {
    EdgeCount degree = p->degree();
    counts[degree] = counts[degree] + 1;
  }
  return counts;
}

DegreeMatrix degree_distribution_matrix(const Graph& graph) {
  DegreeMatrix counts;
  std::vector<EdgeCount> in_degrees = calc_in_degree(graph);
  const VertexList& v = graph.vertices();
  auto q = in_degrees.begin();
  for (auto p = v.begin(); p != v.end(); ++p, ++q) {
    auto degree = std::make_pair(*q, p->degree());
    counts[degree] = counts[degree] + 1;
  }
  return counts;
}

double mean_in_degree(const Graph& graph) {
  double sum = 0.0;
  double n = 0.0;
  std::vector<EdgeCount> in_degree = calc_in_degree(graph);
  for (auto p = in_degree.begin(); p != in_degree.end(); ++p) {
    EdgeCount degree = *p;
    sum += degree;
    n += 1.0;
  }
  return sum/n;
}

double mean_out_degree(const Graph& graph) {
  double sum = 0.0;
  double n = 0.0;
  const VertexList& v = graph.vertices();
  for (auto p = v.begin(); p != v.end(); ++p) {
    EdgeCount degree = p->degree();
    sum += degree;
    n += 1.0;
  }
  return sum/n;
}

