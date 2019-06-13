

#' Create empty graph
#'
#' @return
#' An object of type \code{chickenwire} which is a graph. In this case an empty
#' graph to which edges and vertices can be added using \code{\link{add_edges}} 
#' and \code{\link{add_vertices}}. 
#'
#' @export
create_graph <- function() {
  graph_id <- rcpp_create_graph()
  structure(graph_id, class = "chickenwire")
}

#' Create graph
#'
#' @param vertex_id vector with vertex id's.
#' @param edge_src vector with id's of the starting nodes of the edges. Should
#'   be of the same type as \code{vertex_id}.
#' @param edge_dst vector with id's of the end nodes of the edges. Should
#'   be of the same type as \code{vertex_id}.
#' @param edge_weight numeric vector with edge weights (optional).
#' @param edge_type vector with edge types (optional).
#'
#' @details
#' It is possible to call \code{create_graph} without any argument. An empty
#' graph is then created to which edges and vertices can be added using 
#' \code{\link{add_edges}} and \code{\link{add_vertices}}. 
#' 
#' Vertex id's are stored internally as integers. 
#' When \code{vertex_id} is of type \code{character} or \code{factor} it is 
#' converted to \code{integer}. \code{edge_src} and \code{edge_dst} are also
#' recoded to integer. This is important when later adges or vertices are added
#' using \code{\link{add_edges}} or \code{\link{add_vertices}}. In that case
#' it is necessary to do the conversion to integer manually before calling 
#' \code{create_graph}.
#'
#' @export
create_graph_old <- function(vertex_id, edge_src, edge_dst, edge_weight = 1.0, edge_type = 1L) {
  # when no arguments given; only create the graph; don't add vertices and 
  # edges to the graph
  if (missing(vertex_id) && missing(edge_src) && missing(edge_dst) &&
      missing(edge_weight) && missing(edge_type)) {
    graph_id <- rcpp_create_graph()
    return(structure(graph_id, class = "chickenwire"))
  }
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

