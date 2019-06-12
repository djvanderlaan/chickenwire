
#' Create graph
#'
#' @export
delete_graph <- function(graph_id) {
  stopifnot(is(graph_id, "chickenwire"))
  stopifnot(is.integer(graph_id) && length(graph_id) == 1)
  rcpp_delete_graph(graph_id)
}

