
#include "rcpp_graph_store.h"
#include "graph.h"
#include "random_walk.h"
#include "filter_edges.h"
#include "random_walk.h"

#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::List rcpp_local_average_cont(int graph_id, Rcpp::IntegerVector id, Rcpp::NumericVector value, 
    Rcpp::NumericVector vwght) {
  Graph* graph = get_graph(graph_id);
  // Create values object
  VertexWDoubleValues values;
  // Add vertices to graph and values to values object
  for (R_xlen_t i = 0, j = 0, k =0, end = id.size(); i < end; ++i, ++j, ++k) {
    if (j >= value.size()) j = 0;
    if (k >= vwght.size()) k = 0;
    values[id[i]] = {value[j], vwght[k]};
  }
  // Reweigh graph
  reweigh_edges_by_vertex_and_type(*graph);
  // Random walk
  RandomWalkResult rw = random_walk_continuous(*graph, values, 0.85);
  // Generate result object
  Rcpp::List res(rw.nvalues());
  for (R_xlen_t j = 0, jend = rw.nvalues(); j < jend; ++j) {
    Rcpp::NumericVector v(id.size());
    for (R_xlen_t i = 0, iend = id.size(); i < iend; ++i) {
      v[i] = rw.get(id[i], j);
    } 
    res[j] = v;
  }
  return res;
}

// [[Rcpp::export]]
Rcpp::List rcpp_local_average_cat(int graph_id, Rcpp::IntegerVector id, Rcpp::IntegerVector value, 
    Rcpp::NumericVector vwght) {
  Graph* graph = get_graph(graph_id);
  // Create values object
  VertexWCategoricalValues values;
  // Add vertices to graph and values to values object
  for (R_xlen_t i = 0, j = 0, k =0, end = id.size(); i < end; ++i, ++j, ++k) {
    if (j >= value.size()) j = 0;
    if (k >= vwght.size()) k = 0;
    values[id[i]] = {static_cast<VertexType>(value[j]), vwght[k]};
  }
  // Reweigh graph
  reweigh_edges_by_vertex_and_type(*graph);
  // Random walk
  RandomWalkResult rw = random_walk_categorical(*graph, values, 0.85);
  // Generate result object
  Rcpp::List res(rw.nvalues());
  for (R_xlen_t j = 0, jend = rw.nvalues(); j < jend; ++j) {
    Rcpp::NumericVector v(id.size());
    for (R_xlen_t i = 0, iend = id.size(); i < iend; ++i) {
      v[i] = rw.get(id[i], j);
    } 
    res[j] = v;
  }
  return res;
}

