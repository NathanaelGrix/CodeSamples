# Nathanael Grix

# Set the variable CC to the compiler of choice i
# (gcc for c code, g++ for cpp code)
CC=g++

# Insert compiler flags here (-pg is used for gprof)
CFLAGS= -O3
CXXFLAGS = -O3 -pthread -malign-double #-mtune=native -lmkl_sequential -lmkl_core -lmkl_intel_lp64 -ggdb

DEPS = 

# list out the object files (In this case, two files)
OBJ = hw3_part1_thread_njg5244.o

# Nifty line to compile the source code into object code
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CXXFLAGS)

# Rule to make the executable (link the object codetest_gprof: $(OBJ)
hw3_part1_thread_njg5244: $(OBJ)
	$(CC) -o $@ $^ $(CXXFLAGS)

# cleanup rule (make clean)
.PHONY: clean
clean:
	rm -f $(OBJ) hw3_part1_thread_njg5244
