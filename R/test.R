
library(dplyr)
library(igraph)

devtools::load_all()

data(ssi)
vertices <- ssi$vertices
edges <- ssi$edges


devtools::load_all()
data(ssi)
g <- create_graph(ssi$vertices$id, ssi$edges$src, 
  ssi$edges$dst)
g

rw <- local_average(g, vertices$id, vertex_value = 1*(vertices$color == "pink"))
print(rw)

rw2 <- local_average(g, vertices$id, vertex_value = vertices$color)
print(rw2)

ig <- graph_from_data_frame(edges, directed = TRUE, vertices = vertices)
plot(ig, vertex.size = rw2[[1]]*50)

delete_graph(g)

