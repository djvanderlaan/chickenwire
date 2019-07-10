.PHONY: copy_lib document

chickenwire_headers=graph.h random_walk.h vertex.h edge.h stepper.h chunker.h filter_edges.h types.h shortest_path.h
chickenwire_source=random_walk_categorical.cpp random_walk_continuous.cpp filter_edges.cpp shortest_path.cpp

copy_lib: $(addprefix src/, $(chickenwire_headers)) $(addprefix, src/$(chickenwire_source))
	cp $(addprefix ../include/, $(chickenwire_headers)) src/
	cp $(addprefix ../src/, $(chickenwire_source)) src/

document:
	Rscript  -e "roxygen2::roxygenise()"
