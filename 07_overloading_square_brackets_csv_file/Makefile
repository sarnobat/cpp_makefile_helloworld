# Makefile for stdin line access example

CXX      = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17
TARGET   = stdin
SRC      = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
