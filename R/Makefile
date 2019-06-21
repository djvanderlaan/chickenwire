.PHONY: copy_lib document


copy_lib:
	cp ../include/*.h ./src
	cp ../src/random_walk_categorical.cpp ../src/random_walk_continuous.cpp ../src/filter_edges.cpp ./src

document:
	Rscript  -e "roxygen2::roxygenise()"
