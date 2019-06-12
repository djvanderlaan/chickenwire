
#' Create graph
#' Local network averages
#'
#' @export
create_graph <- function(vertex_id, edge_src, edge_dst, edge_weight = 1.0, edge_type = 1L) {

  # vertex_id
  if (is.factor(vertex_id) || is.character(vertex_id)) {
    edge_src <- match(edge_src, vertex_id)-1
    edge_dst <- match(edge_dst, vertex_id)-1
    vertex_id <- match(vertex_id, vertex_id)-1
  }
  stopifnot(is.numeric(vertex_id) && length(vertex_id) >= 1)
  stopifnot(!any(is.na(vertex_id)))
  # edge_src
  stopifnot(is.numeric(edge_src) && length(edge_src) >= 1)
  stopifnot(!any(is.na(edge_src)))
  # edge_dst
  stopifnot(is.numeric(edge_dst) && length(edge_dst) >= 1)
  stopifnot(length(edge_dst) == length(edge_src))
  stopifnot(!any(is.na(edge_dst)))
  # edge_weight
  stopifnot(is.numeric(edge_weight) && length(edge_weight) >= 1)
  stopifnot(length(edge_weight == 1) || length(edge_weight) == length(edge_src))
  stopifnot(!any(is.na(edge_weight)))
  # edge_type
  if (is.factor(edge_type) || is.character(edge_type)) {
    edge_type <- as.integer(as.factor(edge_type)) - 1
  }
  stopifnot(is.numeric(edge_type) && length(edge_type) >= 1)
  stopifnot(length(edge_type == 1) || length(edge_type) == length(edge_src))
  stopifnot(!any(is.na(edge_type)))
  # create
  graph_id <- rcpp_create_graph()
  rcpp_add_vertices(graph_id, vertex_id)
  rcpp_add_edges(graph_id, edge_src, edge_dst, edge_weight, edge_type, FALSE)
  structure(graph_id, class = "chickenwire")
}

