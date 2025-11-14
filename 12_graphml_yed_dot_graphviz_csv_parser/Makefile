CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I/opt/homebrew/include
LDFLAGS  = -L/opt/homebrew/lib
LDLIBS   = -ltinyxml2

TARGET = graphml_reader
SRC = main.cpp

run:
	./graphml_reader /Volumes/git/sarnobat.git/yed/trail_lisp.graphml

all: deps $(TARGET)	run

.PHONY: deps
deps:
	@brew list tinyxml2 >/dev/null 2>&1 || brew install tinyxml2



$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(TARGET) *.o
