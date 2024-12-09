CXX = g++

CXXFLAGS = -std=c++23 -Wall -Wextra -O2 

d1:
	$(CXX) $(CXXFLAGS) "days/1.cpp" -o "bin/1.out"
d2:
	$(CXX) $(CXXFLAGS) "days/2.cpp" -o "bin/2.out"
d3:
	$(CXX) $(CXXFLAGS) "days/3.cpp" -o "bin/3.out"
d4:
	$(CXX) $(CXXFLAGS) "days/4.cpp" -o "bin/4.out"
d5:
	$(CXX) $(CXXFLAGS) "days/5.cpp" -o "bin/5.out"
d6:
	$(CXX) $(CXXFLAGS) "days/6.cpp" -o "bin/6.out"
d7:
	$(CXX) $(CXXFLAGS) "days/7.cpp" -o "bin/7.out"
d8:
	$(CXX) $(CXXFLAGS) "days/8.cpp" -o "bin/8.out"

clean:
	find . -type f -name "*.out" -delete

.PHONY: clean help