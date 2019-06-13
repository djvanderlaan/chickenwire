

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

