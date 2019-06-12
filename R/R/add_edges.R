
#' Add edges
#'
#' @export
add_edges <- function(graph_id, edges, vertices, edge_src_col = 1, edge_dst_col = 2, 
    edge_weight_col, edge_type_col, vertex_id_col = 1, auto_add_vertices = TRUE) {
  # Check input
  stopifnot(is(graph_id, "chickenwire"))
  stopifnot(is.integer(graph_id) && length(graph_id) == 1)

  edge_src <- edges[, edge_src_col]
  edge_dst <- edges[, edge_dst_col]
  if (!missing(vertices)) {
    if (is.numeric(vertices)) {
      vertex_id <- vertices
    } else {
      vertex_id <- vertices[, vertex_id_col]
    }
  }
  edge_weight <- if (!missing(edge_weight_col)) 
    edges[, edge_weight_col] else 1.0
  edge_type <- if (!missing(edge_type_col)) 
    edges[, edge_type_col] else 1L
  if (is.factor(edge_type) || is.character(edge_type)) {
    edge_type <- as.integer(as.factor(edge_type)) - 1
  }
  check_edges(edge_src, edge_dst, edge_weight, edge_type)

  if (missing(vertices)) {
    rcpp_add_edges(graph_id, edge_src, edge_dst, edge_weight, edge_type, 
      auto_add_vertices)
  } else {
    rcpp_add_vertices(graph_id, vertex_id)
    rcpp_add_edges(graph_id, edge_src, edge_dst, edge_weight, edge_type, FALSE)
  }
  graph_id
}

