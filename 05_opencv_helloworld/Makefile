# Makefile for hello_world_opencv

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra `pkg-config --cflags opencv4`

# Libraries to link
LDLIBS := `pkg-config --libs opencv4`

# Target executable name
TARGET := hello_opencv

# Source files
SRC := main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGET)

# force to execute (even if already made?)
.PHONY: all clean
