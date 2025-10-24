# -------------------------------
# Bouncing Ball OpenGL Makefile
# -------------------------------

CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall

HOMEBREW_PREFIX := $(shell brew --prefix)

# Include paths (use glad folder as-is)
INCLUDES := -I. \
            -I$(HOMEBREW_PREFIX)/include \
            -I$(HOMEBREW_PREFIX)/Cellar/glfw/3.4/include \
            -Iglad \
            -Iglad/KHR \
            -Iglad/glad

# Libraries
LIBS := -L$(HOMEBREW_PREFIX)/lib -lglfw -framework OpenGL

# Sources
SRC := main.cpp glad/glad.c
OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

TARGET := bouncing_ball

# -------------------------------
# Default target
# -------------------------------
all: show_glad_origin setup $(TARGET)

# -------------------------------
# Show GLAD zip origin in output
# -------------------------------
show_glad_origin:
	@echo "GLAD zip used for this build: https://glad.dav1d.de/generated/tmpl_uuw6hqglad/"

# -------------------------------
# Setup dependencies
# -------------------------------
setup: install_homebrew_libs glad

install_homebrew_libs:
	@echo "Checking Homebrew packages..."
	@if ! brew list glfw >/dev/null 2>&1; then \
		echo "Installing GLFW..."; brew install glfw; \
	else \
		echo "GLFW already installed."; \
	fi
	@if ! brew list glm >/dev/null 2>&1; then \
		echo "Installing GLM..."; brew install glm; \
	else \
		echo "GLM already installed."; \
	fi

# Extract GLAD only if folder does not exist
glad:
	@if [ ! -d glad ]; then \
		echo "Extracting GLAD from glad.zip..."; \
		unzip -o glad.zip; \
		echo "GLAD extraction complete."; \
	else \
		echo "GLAD already exists."; \
	fi

# -------------------------------
# Build target
# -------------------------------
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LIBS)

# Compile C++ sources
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile C sources
%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all show_glad_origin setup install_homebrew_libs glad clean
