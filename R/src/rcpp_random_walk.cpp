
#include <Rcpp.h>

#include "graph.h"
#include "functions.h"
#include "filter_edges.h"
#include "random_walk.h"

// [[Rcpp::export]]
Rcpp::List rcpp_random_walk(Rcpp::IntegerVector id, Rcpp::NumericVector value, Rcpp::NumericVector vwght, 
    Rcpp::IntegerVector src, Rcpp::IntegerVector dst, Rcpp::NumericVector wght, Rcpp::IntegerVector type) {
  // Create graph and values object
  Graph graph(true);
  VertexWDoubleValues values;
  // Add vertices to graph and values to values object
  for (R_xlen_t i = 0, j = 0, k =0, end = id.size(); i < end; ++i, ++j, ++k) {
    graph.add_vertex(id[i]);
    if (j >= value.size()) j = 0;
    if (k >= vwght.size()) j = 0;
    values[id[i]] = {value[j], vwght[k]};
  }
  // Add edges to graph
  for (R_xlen_t i = 0, j = 0, k = 0, end = src.size(); i != end; ++i, ++j, ++k) {
    if (j >= wght.size()) j = 0;
    if (k >= wght.size()) k = 0;
    graph.add_edge(src[i], dst[i], wght[j]);
  }
  // Reweigh graph
  reweigh_edges_by_vertex_and_type(graph);
  // Random walk
  RandomWalkResult rw = random_walk_continuous(graph, values, 0.85);
  // Generate result object
  Rcpp::List res(rw.nvalues());
  for (R_xlen_t j = 0, jend = rw.nvalues(); j < jend; ++j) {
    Rcpp::NumericVector v(id.size());
    for (R_xlen_t i = 0, iend = id.size(); i < iend; ++i) 
      v[i] = rw.get(i, j);
    res[j] = v;
  }
  return res;
}

