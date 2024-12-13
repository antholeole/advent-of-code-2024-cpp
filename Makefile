CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra `pkg-config --libs gtest` -g -D DEBUG

NIX_CFLAGS_COMPILE	?= -I /usr/local/include

# uses any target, it doesn't matter. This gives you the required files for clangd support.
compile_commands:
	bear -- $(CXX) $(CXXFLAGS) $(CFLAGS) $(NIX_CFLAGS_COMPILE) -D MAIN  "days/1.cpp" -o "bin/1.out" 
sanity_tests:
	$(CXX) $(CXXFLAGS) "days/sanity.test.cpp" -o "bin/sanity.test.out" -D TEST_MAIN

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
	$(CXX) $(CXXFLAGS) "days/9.test.cpp" -o "bin/9.out" -D MAIN
d9t:
	$(CXX) $(CXXFLAGS) "days/9.test.cpp" -o "bin/9.test.out" -D TEST_MAIN


d10:
	$(CXX) $(CXXFLAGS) "days/10.test.cpp" -o "bin/10.out" -D MAIN
d10t:
	$(CXX) $(CXXFLAGS) "days/10.test.cpp" -o "bin/10.test.out" -D TEST_MAIN

d11:
	$(CXX) $(CXXFLAGS) "days/11.test.cpp" -o "bin/11.out" -D MAIN
d11t:
	$(CXX) $(CXXFLAGS) "days/11.test.cpp" -o "bin/11.test.out" -D TEST_MAIN

clean:
	find . -type f -name "*.out" -delete
.PHONY: clean 