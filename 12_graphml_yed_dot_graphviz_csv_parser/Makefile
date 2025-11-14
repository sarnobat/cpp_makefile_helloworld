# Makefile for GraphML CSV tools

CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -ltinyxml2

GRAPHML = /Volumes/git/sarnobat.git/yed/trail_lisp.graphml

NODES = nodes
EDGES = edges


run: all
	@echo "=== Nodes CSV (full) ==="
	./$(NODES) $(GRAPHML)
	@echo ""
	@echo "=== Nodes CSV (--short) ==="
	./$(NODES) --short $(GRAPHML)
	@echo ""
	@echo "=== Edges CSV (full) ==="
	./$(EDGES) $(GRAPHML)
	@echo ""
	@echo "=== Edges CSV (--short) ==="
	./$(EDGES) --short $(GRAPHML)

all: $(NODES) $(EDGES)

$(NODES): nodes.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

$(EDGES): edges.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)


clean:
	rm -f $(NODES) $(EDGES)
