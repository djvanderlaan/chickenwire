#include "functions.h"
#include <utility>
#include <unordered_map>

inline std::unordered_map<vertex_id, EdgeCount> calc_in_degree(const Graph& graph) {
  std::unordered_map<vertex_id, EdgeCount> in_degree;
  const VertexList& vertices = graph.vertices();
  for (auto vertp = vertices.begin(); vertp != vertices.end(); ++vertp) {
    const EdgeList& edges = vertp->second.edges();
    for (auto edgep = edges.begin(); edgep != edges.end(); ++edgep) {
      in_degree[edgep->dst()] += 1;
    }
  }
  return in_degree;
}

DegreeDistribution in_degree_distribution(const Graph& graph) {
  std::unordered_map<vertex_id, EdgeCount> in_degree = calc_in_degree(graph);
  DegreeDistribution counts;
  for (auto p = in_degree.begin(); p != in_degree.end(); ++p) {
    EdgeCount degree = p->second;
    counts[degree] = counts[degree] + 1;
  }
  return counts;
}

DegreeDistribution out_degree_distribution(const Graph& graph) {
  DegreeDistribution counts;
  const VertexList& v = graph.vertices();
  for (auto p = v.begin(); p != v.end(); ++p) {
    EdgeCount degree = p->second.degree();
    counts[degree] = counts[degree] + 1;
  }
  return counts;
}

DegreeMatrix degree_distribution_matrix(const Graph& graph) {
  DegreeMatrix counts;
  std::unordered_map<vertex_id, EdgeCount> in_degrees = calc_in_degree(graph);
  const VertexList& v = graph.vertices();
  for (auto p = v.begin(); p != v.end(); ++p) {
    EdgeCount out_degree = p->second.degree();
    EdgeCount in_degree = in_degrees[p->first];
    auto degree = std::make_pair(in_degree, out_degree);
    counts[degree] = counts[degree] + 1;
  }
  return counts;
}

double mean_in_degree(const Graph& graph) {
  double sum = 0.0;
  double n = 0.0;
  std::unordered_map<vertex_id, EdgeCount> in_degree = calc_in_degree(graph);
  for (auto p = in_degree.begin(); p != in_degree.end(); ++p) {
    EdgeCount degree = p->second;
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
    EdgeCount degree = p->second.degree();
    sum += degree;
    n += 1.0;
  }
  return sum/n;
}

