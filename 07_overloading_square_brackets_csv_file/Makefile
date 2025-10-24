# Makefile for stdin pipeline example

CXX      = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17
TARGET   = pipeline
SRC      = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

run:
	seq 990 1010 | ./pipeline

clean:
	rm -f $(TARGET)
