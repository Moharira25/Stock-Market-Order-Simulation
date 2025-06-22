#Modified version of the makefile from assignment 1:
# Specify the C++ compiler
CXX     = g++

# Specify options to pass to the compiler. Here it sets the optimisation
# level, outputs debugging info for gdb, and C++ version to use.
CXXFLAGS = -O0 -g3 -std=c++17

All: all
all: main

# This is the executable to be produced
main: main.o order.o exchange.o
	$(CXX) $(CXXFLAGS) main.o order.o exchange.o -o main

# These are the "intermediate" object files
# The -c command produces them
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

order.o: order.cpp
	$(CXX) $(CXXFLAGS) -c order.cpp -o order.o

exchange.o: exchange.cpp
	$(CXX) $(CXXFLAGS) -c exchange.cpp -o exchange.o

# Some cleanup functions, invoked by typing "make clean" or "make deepclean"
deepclean:
	rm -f *~ *.o main main.exe *.stackdump

clean:
	rm -f *~ *.o *.stackdump
