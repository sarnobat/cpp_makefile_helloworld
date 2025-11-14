CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I/opt/homebrew/include
LDFLAGS  = -L/opt/homebrew/lib
LDLIBS   = -lboost_graph -lboost_system -lboost_filesystem -lboost_serialization -ltinyxml2

TARGET = graphml_reader
SRC = main.cpp

# Install dependencies via Homebrew
.PHONY: deps
deps:
	@echo "Installing dependencies via Homebrew..."
	@brew list boost >/dev/null 2>&1 || brew install boost
	@brew list tinyxml2 >/dev/null 2>&1 || brew install tinyxml2

# Default target: install deps and build
all: deps $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(TARGET) *.o
