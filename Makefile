CXX = g++

CXXFLAGS = -std=c++23 -Wall -Wextra -O2 

d1:
	$(CXX) $(CXXFLAGS) "days/1.cpp" -o "bin/1.out"
d2:
	$(CXX) $(CXXFLAGS) "days/2.cpp" -o "bin/2.out"
d3:
	$(CXX) $(CXXFLAGS) "days/3.cpp" -o "bin/3.out"

clean:
	find . -type f -name "*.out" -delete

.PHONY: clean help