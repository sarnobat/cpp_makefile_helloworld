# -------------------------------
# Bouncing Ball OpenGL Makefile (Static GLFW, Fixed QuartzCore)
# -------------------------------

CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall

HOMEBREW_PREFIX := $(shell brew --prefix)

# Include paths
INCLUDES := -I. \
            -I$(HOMEBREW_PREFIX)/include \
            -Iglad \
            -Iglad/KHR \
            -Iglad/glad \
            -Iexternal/glfw/include

# Static GLFW and required macOS frameworks
LIBS := external/glfw/src/libglfw3.a \
        -framework Cocoa -framework IOKit -framework CoreVideo \
        -framework OpenGL -framework QuartzCore

SRC := main.cpp glad/glad.c
OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

TARGET := bouncing_ball

# -------------------------------
# Default target
# -------------------------------
all: show_glad_origin setup glfw_static $(TARGET)

show_glad_origin:
	@echo "GLAD zip used for this build: https://glad.dav1d.de/generated/tmpl_uuw6hqglad/"

# -------------------------------
# Setup
# -------------------------------
setup: install_homebrew_libs glad

install_homebrew_libs:
	@echo "Checking Homebrew packages..."
	@if ! brew list glm >/dev/null 2>&1; then \
		echo "Installing GLM..."; brew install glm; \
	else \
		echo "GLM already installed."; \
	fi

glad:
	@if [ ! -d glad ]; then \
		echo "Extracting GLAD from glad.zip..."; \
		unzip -o glad.zip; \
		echo "GLAD extraction complete."; \
	else \
		echo "GLAD already exists."; \
	fi

# -------------------------------
# Build static GLFW
# -------------------------------
glfw_static:
	@if [ ! -f external/glfw/src/libglfw3.a ]; then \
		echo "Building static GLFW..."; \
		mkdir -p external; cd external; \
		if [ ! -d glfw ]; then \
			git clone https://github.com/glfw/glfw.git; \
		fi; \
		cd glfw; \
		cmake -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF \
		      -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF .; \
		make -j$$(sysctl -n hw.ncpu); \
	else \
		echo "Static GLFW already built."; \
	fi

# -------------------------------
# Link and compile
# -------------------------------
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all show_glad_origin setup install_homebrew_libs glad glfw_static clean
