COMPILER = g++-4.7
LINKER = g++-4.7
FLAGS = -g -Wall -std=c++11 -O3
TEST_LINKS = -lgtest -lpthread

VORONOI_GENERATOR_OBJS = vg_main.o voronoi.o voronoi_event.o voronoi_cell.o voronoi_generator.o mpvec3.o mp_sample_generator.o beachline.o priqueue.o
TEST_OBJS = tests.o voronoi.o voronoi_event.o voronoi_cell.o mp_sample_generator.o mpvec3.o beachline.o priqueue.o


vg: $(VORONOI_GENERATOR_OBJS)
	$(LINKER) $(VORONOI_GENERATOR_OBJS) -o vg

tests: $(TEST_OBJS)
	$(LINKER) $(TEST_OBJS) $(TEST_LINKS) -o tests

all: vg tests

vg_main.o: vg_main.cpp
	$(COMPILER) vg_main.cpp $(FLAGS) -c

mp_sample_generator.o: src/mp_sample_generator.h src/mp_sample_generator.cpp
	$(COMPILER) src/mp_sample_generator.cpp $(FLAGS) -c

voronoi_event.o: src/voronoi_event.h src/voronoi_event.cpp
	$(COMPILER) src/voronoi_event.cpp $(FLAGS) -c

voronoi.o: src/voronoi.h src/voronoi.cpp src/voronoi_event_compare.h
	$(COMPILER) src/voronoi.cpp $(FLAGS) -c

beachline.o: src/beachline.h src/beachline.cpp
	$(COMPILER) src/beachline.cpp $(FLAGS) -c

priqueue.o: src/priqueue.h src/priqueue.cpp src/voronoi_event_compare.h
	$(COMPILER) src/priqueue.cpp $(FLAGS) -c

voronoi_cell.o: src/voronoi_cell.h src/voronoi_cell.cpp
	$(COMPILER) src/voronoi_cell.cpp $(FLAGS) -c

mpvec3.o: src/mpreal/mpvec3.h src/mpreal/mpvec3.cpp
	$(COMPILER) src/mpreal/mpvec3.cpp $(FLAGS) -c

voronoi_generator.o: src/voronoi_generator.h src/voronoi_generator.cpp
	$(COMPILER) src/voronoi_generator.cpp $(FLAGS) -c

tests.o: test/tests.cpp test/voronoi_tests.cpp test/priqueue_tests.cpp
	$(COMPILER) test/tests.cpp $(FLAGS) -c


.PHONY: clean spaces

clean:
	rm *.o vg tests 2>/dev/null

spaces:
	find . \( -name '*.h' -o -name '*.cpp' \) ! -type d -exec bash -c 'expand -i -t 4 "$$0" > /tmp/e && mv /tmp/e "$$0"' {} \;
