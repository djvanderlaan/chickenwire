.PHONY: copy_lib document clean

lib_headers= $(notdir $(wildcard ../include/*.h))
lib_source= $(notdir $(wildcard ../src/*.cpp))

copy_lib: $(addprefix ../include/, $(lib_headers)) $(addprefix, include/, $(lib_source))
	cp $(addprefix ../include/, $(lib_headers)) src/
	cp $(addprefix ../src/, $(lib_source)) src/

clean:
	rm -f $(addprefix src/, $(lib_headers)) $(addprefix src/, $(lib_source)) 

document:
	Rscript  -e "roxygen2::roxygenise()"
