#include "functions.h"
#include "rcpp_graph_store.h"

#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::IntegerVector rcpp_graph_size(int graph_id) {
  const Graph* graph = get_graph(graph_id);
  Rcpp::IntegerVector res(1);
  res[0] = graph->vertices().size();
  return res;
}

