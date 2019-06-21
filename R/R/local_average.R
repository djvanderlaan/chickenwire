#' Local network averages
#'
#' @param graph_id the graph to which to add the edges and vertices. Should
#'   be of type \code{chickenwire}.
#' @param vertex_id a vector with vertex id's. Should contain integer values.
#' @param vertex_value a vector with vertex values. Can be numeric, factor or
#'   character values. Should have the same length as \code{vertex_id}.
#' @param vertex_weight a vector with vertex weights. Should be numeric and 
#'   have the same length as \code{vertex_id}. Optional.
#'
#' @return
#' A data.frame. This first column contains the vertex id's. 
#'
#' @export
local_average <- function(graph_id, vertex_id, vertex_value, vertex_weight = 1.0) {

  stopifnot(methods::is(graph_id, "chickenwire"))
  stopifnot(is.integer(graph_id) && length(graph_id) == 1)

  # vertex_id
  if (is.factor(vertex_id) || is.character(vertex_id)) 
    vertex_id <- seq_along(vertex_id)-1L
  stopifnot(is.numeric(vertex_id) && length(vertex_id) >= 1)
  stopifnot(!any(is.na(vertex_id)))
  # vertex_weight
  stopifnot(is.numeric(vertex_weight) && length(vertex_weight) >= 1)
  stopifnot(length(vertex_weight == 1) || length(vertex_weight) == length(vertex_id))
  stopifnot(!any(is.na(vertex_weight)))
  # vertex_value
  value_name <- deparse(substitute(vertex_value))
  value_factor <- FALSE
  if (is.factor(vertex_value) || is.character(vertex_value)) {
    vertex_value <- as.factor(vertex_value)
    value_name <- levels(vertex_value)
    vertex_value <- as.integer(vertex_value) - 1
    value_factor <- TRUE
  } else if (is.logical(vertex_value)) {
    vertex_value <- 1.0 * vertex_value;
  }
  stopifnot(is.numeric(vertex_value) && length(vertex_value) >= 1)
  stopifnot(length(vertex_value == 1) || length(vertex_value) == length(vertex_id))
  stopifnot(!any(is.na(vertex_value)))
  # random_walk
  if (value_factor) {
    res <- rcpp_local_average_cat(graph_id, vertex_id, vertex_value, vertex_weight)
  } else {
    res <- rcpp_local_average_cont(graph_id, vertex_id, vertex_value, vertex_weight)
  }
  res <- as.data.frame(res)
  names(res) <- value_name
  res
}

