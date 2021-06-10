# Nathanael Grix

# Set the variable CC to the compiler of choice i
# (gcc for c code, g++ for cpp code)
CC=g++

# Insert compiler flags here (-pg is used for gprof)
CFLAGS= -O3
CXXFLAGS = -O3

DEPS = 

# list out the object files (In this case, two files)
OBJ = hw2_task2_njg5244.o

# Nifty line to compile the source code into object code
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to make the executable (link the object codetest_gprof: $(OBJ)
hw2_task2_njg5244: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# cleanup rule (make clean)
.PHONY: clean
clean:
	rm -f $(OBJ) hw2_task2_njg5244
