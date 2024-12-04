CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2  `pkg-config --libs gtest`

NIX_CFLAGS_COMPILE	?= -I /usr/local/include

# uses any target, it doesn't matter. This gives you the required files for clangd support.
compile_commands:
	bear -- $(CXX) $(CXXFLAGS) $(CFLAGS) $(NIX_CFLAGS_COMPILE) "days/1.cpp" -o "bin/1.out" 

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
d9:
	$(CXX) $(CXXFLAGS) "days/9.cpp" -o "bin/9.out"

clean:
	find . -type f -name "*.out" -delete
.PHONY: clean 