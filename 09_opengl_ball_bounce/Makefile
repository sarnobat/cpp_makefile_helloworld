# -------------------------------
# Bouncing Ball OpenGL Makefile
# -------------------------------

CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall

HOMEBREW_PREFIX := $(shell brew --prefix)

# Include paths
INCLUDES := -I. \
            -I$(HOMEBREW_PREFIX)/include \
            -I$(HOMEBREW_PREFIX)/Cellar/glfw/3.4/include

# Libraries
LIBS := -L$(HOMEBREW_PREFIX)/lib -lglfw -framework OpenGL

# GLAD
GLAD_DIR := glad
GLAD_ZIP := glad.zip
GLAD_SRC := $(GLAD_DIR)/glad.c
GLAD_HEADERS := $(GLAD_DIR)/glad.h $(GLAD_DIR)/KHR/khrplatform.h

# Sources
SRC := main.cpp $(GLAD_SRC)
OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

# Target
TARGET := bouncing_ball

# -------------------------------
# Default target
# -------------------------------
all: setup $(TARGET)

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

glad:
	@if [ ! -f $(GLAD_SRC) ]; then \
		echo "Extracting GLAD from $(GLAD_ZIP)..."; \
		unzip -o $(GLAD_ZIP) -d temp_glad; \
		mkdir -p $(GLAD_DIR)/KHR $(GLAD_DIR)/glad; \
		cp temp_glad/include/glad/glad.h $(GLAD_DIR)/glad/; \
		cp temp_glad/include/KHR/khrplatform.h $(GLAD_DIR)/KHR/; \
		cp temp_glad/src/glad.c $(GLAD_DIR)/; \
		rm -rf temp_glad; \
		echo "GLAD setup complete."; \
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

.PHONY: all setup install_homebrew_libs glad clean
