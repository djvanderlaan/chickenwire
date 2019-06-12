#include "rcpp_graph_store.h"
#include <Rcpp.h>

#include <iostream>
#include <iomanip>

// [[Rcpp::export]]
int rcpp_create_graph() {
  Graph* graph = new Graph(true);
  return register_graph(graph);
}

// [[Rcpp::export]]
void rcpp_add_vertices(int graph_id, Rcpp::IntegerVector id) {
  Graph* graph = get_graph(graph_id);
  for (R_xlen_t i = 0, end = id.size(); i < end; ++i) 
    graph->add_vertex(id[i]);
}

// [[Rcpp::export]]
void rcpp_add_edges(int graph_id, Rcpp::IntegerVector src, Rcpp::IntegerVector dst, 
    Rcpp::NumericVector wght, Rcpp::IntegerVector type, bool auto_add_vertex) {
  Graph* graph = get_graph(graph_id);
  for (R_xlen_t i = 0, j = 0, k = 0, end = src.size(); i != end; ++i, ++j, ++k) {
    if (j >= wght.size()) j = 0;
    if (k >= type.size()) k = 0;
    graph->add_edge(src[i], dst[i], wght[j], type[k]);
  }
}

// [[Rcpp::export]]
void rcpp_delete_graph(int graph_id) {
  delete_graph(graph_id);
}

