#include "functions.h"
#include <utility>
#include <unordered_map>

DegreeDistribution in_degree_distribution(const Graph& graph) {
  std::unordered_map<vertex_id, unsigned int> in_degree;
  const VertexList& vertices = graph.vertices();
  for (auto vertp = vertices.begin(); vertp != vertices.end(); ++vertp) {
    const EdgeList& edges = vertp->second.edges();
    for (auto edgep = edges.begin(); edgep != edges.end(); ++edgep) {
      in_degree[edgep.dst()] += 1;
    }
  }

  DegreeDistribution counts;
  for (auto p = in_degree.begin(); 



  //const VertexList& v = graph.vertices();
  //for (auto p = v.begin(); p != v.end(); ++p) {
    //EdgeCount degree = p->second.in_degree();
    //counts[degree] = counts[degree] + 1;
  //}
  //TODO
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
  //const VertexList& v = graph.vertices();
  //for (auto p = v.begin(); p != v.end(); ++p) {
    //EdgeCount out_degree = p->second.out_degree();
    //EdgeCount in_degree = p->second.in_degree();
    //auto degree = std::make_pair(in_degree, out_degree);
    //counts[degree] = counts[degree] + 1;
  //}
  // TODO
  return counts;
}

double mean_in_degree(const Graph& graph) {
  double sum = 0.0;
  double n = 0.0;
  //const VertexList& v = graph.vertices();
  //for (auto p = v.begin(); p != v.end(); ++p) {
    //EdgeCount degree = p->second.in_degree();
    //sum += degree;
    //n += 1.0;
  //}
  // TODO
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

