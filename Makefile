CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2 

# nix if it exists, global if it doesn't.
NIX_CFLAGS_COMPILE	?= -I /usr/local/include
CFLAGS	+= $(NIX_CFLAGS_COMPILE)
NIX_LDFLAGS		?= -L /usr/local/lib
LDFLAGS		+= $(NIX_LDFLAGS)


# uses any target, it doesn't matter. This gives you the required files for clangd support.
compile_commands:
	bear -- $(CXX) $(CXXFLAGS) $(CFLAGS) "days/1.cpp" -o "bin/1.out" 

d1:
	$(CXX) $(CXXFLAGS) "days/1.cpp" -o "bin/1.out"
d2:
	$(CXX) $(CXXFLAGS) "days/2.cpp" -o "bin/2.out"
d3:
	$(CXX) $(CXXFLAGS) "days/3.cpp" -o "bin/3.out"
d4:
	$(CXX) $(CXXFLAGS) $(CFLAGS) "days/4.cpp" -o "bin/4.out"


clean:
	find . -type f -name "*.out" -delete
.PHONY: clean 