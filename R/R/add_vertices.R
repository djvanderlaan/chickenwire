
#' Add vertices
#'
#' @export
add_vertices <- function(graph_id, vertices, vertex_id_col = 1L) {
  # Check input
  stopifnot(is(graph_id, "chickenwire"))
  stopifnot(is.integer(graph_id) && length(graph_id) == 1)
  if (!missing(vertices)) {
    if (is.numeric(vertices)) {
      vertex_id <- vertices
    } else {
      vertex_id <- vertices[, vertex_id_col]
    }
  }
  stopifnot(is.numeric(vertex_id) && length(vertex_id) >= 1)
  stopifnot(!any(is.na(vertex_id)))
  # Add
  rcpp_add_vertices(graph_id, vertex_id)
  graph_id
}

