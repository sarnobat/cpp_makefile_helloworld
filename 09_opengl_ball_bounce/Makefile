# ----------------------------------------------------------
# Bouncing Ball OpenGL Makefile (macOS, static GLFW build)
# ----------------------------------------------------------
# GLAD zip used: https://glad.dav1d.de/generated/tmpl_uuw6hqglad/
# ----------------------------------------------------------

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

# Library/framework dependencies
LIBS := external/glfw/build/src/libglfw3.a \
        -framework Cocoa -framework IOKit -framework CoreVideo \
        -framework QuartzCore -framework OpenGL

SRC := main.cpp glad/glad.c
OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

TARGET := bouncing_ball

# ----------------------------------------------------------
# Default target
# ----------------------------------------------------------
all: show_glad_origin setup $(TARGET)

# ----------------------------------------------------------
# Show GLAD source
# ----------------------------------------------------------
show_glad_origin:
	@echo "GLAD zip used for this build: https://glad.dav1d.de/generated/tmpl_uuw6hqglad/"

# ----------------------------------------------------------
# Dependency setup
# ----------------------------------------------------------
setup: install_homebrew_libs glad glfw_static

install_homebrew_libs:
	@echo "Checking Homebrew packages..."
	@if ! brew list glm >/dev/null 2>&1; then \
		echo "Installing GLM..."; brew install glm; \
	else \
		echo "GLM already installed."; \
	fi

# Unpack GLAD headers and sources if missing
glad:
	@if [ ! -d glad ]; then \
		echo "Extracting GLAD from glad.zip..."; \
		unzip -o glad.zip; \
		echo "GLAD extraction complete."; \
	else \
		echo "GLAD already exists."; \
	fi

# ----------------------------------------------------------
# GLFW (static build)
# ----------------------------------------------------------
glfw_static:
	@if [ ! -d external/glfw ]; then \
		echo "Adding GLFW submodule..."; \
		git submodule add https://github.com/glfw/glfw.git external/glfw || true; \
	fi
	@if [ ! -f external/glfw/CMakeLists.txt ]; then \
		echo "Initializing GLFW submodule..."; \
		git submodule update --init --recursive external/glfw; \
	fi
	@if [ ! -f external/glfw/build/src/libglfw3.a ]; then \
		echo "Building static GLFW..."; \
		mkdir -p external/glfw/build && cd external/glfw && \
		cmake -S . -B build -DBUILD_SHARED_LIBS=OFF \
			-DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF; \
		cmake --build build; \
	else \
		echo "Static libglfw3.a already built."; \
	fi

# ----------------------------------------------------------
# Build the target
# ----------------------------------------------------------
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ----------------------------------------------------------
# Clean
# ----------------------------------------------------------
clean:
	rm -f $(OBJ) $(TARGET)
	@echo "Keeping external/glfw intact."

.PHONY: all show_glad_origin setup install_homebrew_libs glad glfw_static clean
