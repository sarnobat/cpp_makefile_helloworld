CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall

HOMEBREW_PREFIX := $(shell brew --prefix)

INCLUDES := -I. \
            -I$(HOMEBREW_PREFIX)/include \
            -I$(HOMEBREW_PREFIX)/Cellar/glfw/3.4/include

LIBS := -L$(HOMEBREW_PREFIX)/lib -lglfw -framework OpenGL

SRC := main.cpp glad.c
OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

TARGET := bouncing_ball

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
