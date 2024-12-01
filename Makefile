CXX = clang++

CXXFLAGS = -std=c++2b -Wall -Wextra -O2

d1:
	$(CXX) $(CXXFLAGS) "days/1.cpp" -o "bin/1.out"

clean:
	find . -type f -name "*.out" -delete

.PHONY: clean help