# Makefile for GraphML CSV tools, DOT converter, and Graph::Easy ASCII rendering

CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -ltinyxml2

# GRAPHML = /Volumes/git/sarnobat.git/yed/trail_lisp.graphml
GRAPHML = '/Volumes/git/sarnobat.git/yed/disruptive innovation cannibalization.graphml'

NODES = nodes
EDGES = edges
CSV2DOT = csv2dot

GRAPH_EASY_BIN = /Volumes/Apps/Graph-Easy-0.64/bin/graph-easy
GRAPH_EASY_LIB = /Volumes/Apps/Graph-Easy-0.64/lib

all: $(NODES) $(EDGES) $(CSV2DOT)

# Compile nodes CSV program
$(NODES): nodes.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Compile edges CSV program
$(EDGES): edges.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Compile CSV to DOT converter
$(CSV2DOT): csv2dot.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Run all CSV programs and DOT converter
run: all
	@echo "=== Nodes CSV (full) ==="
	./$(NODES) $(GRAPHML) | tee nodes.csv
	@echo ""
	@echo "=== Nodes CSV (--short) ==="
	./$(NODES) --short $(GRAPHML) | tee nodes_short.csv
	@echo ""
	@echo "=== Edges CSV (full) ==="
	./$(EDGES) $(GRAPHML) | tee edges.csv
	@echo ""
	@echo "=== Edges CSV (--short) ==="
	./$(EDGES) --short $(GRAPHML) | tee edges_short.csv
	@echo ""
	@echo "=== DOT graph (full IDs) ==="
	./$(CSV2DOT) nodes.csv edges.csv | tee graph.dot
	@echo "DOT output written to graph.dot"
	@echo ""
	@echo "=== DOT graph (--short IDs) ==="
	./$(CSV2DOT) nodes_short.csv edges_short.csv | tee graph_short.dot
	dot -Tpng  graph_short.dot > graph_short.png
	open graph_short.png
	@echo "DOT output written to graph_short.dot"
	@echo ""
	@echo "=== Graph::Easy ASCII rendering ==="
	cat graph_short.dot | perl -I $(GRAPH_EASY_LIB) $(GRAPH_EASY_BIN) --from=dot --as_ascii | tee graphviz.grapheasy.txt.out

clean:
	rm -f $(NODES) $(EDGES) $(CSV2DOT)
	rm -f nodes.csv nodes_short.csv edges.csv edges_short.csv
	rm -f graph.dot graph_short.dot graphviz.grapheasy.txt.out
