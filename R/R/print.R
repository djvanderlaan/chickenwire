

#' @export
print.chickenwire <- function(x, ...) {
  stopifnot(is(x, "chickenwire"))
  stopifnot(is.integer(x) && length(x) == 1)
  print(rcpp_graph_size(x))
}
