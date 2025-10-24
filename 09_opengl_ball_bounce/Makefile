CXX = g++
CXXFLAGS = -std=c++17 -O2 -I.
TARGET = bouncing_ball
SOURCES = main.cpp glad.c
OBJECTS = main.o glad.o

# GLAD variables
GLAD_ZIP_URL = https://github.com/Dav1dde/glad/archive/refs/tags/v2.0.8.zip
GLAD_DIR = glad-2.0.8
GLAD_GENERATOR = $(GLAD_DIR)/utility/generateall.sh

all: glad $(TARGET)

# Download, generate, and copy GLAD files
glad:
	@echo "==> Fetching GLAD v2.0.8 from GitHub..."
	@rm -f glad.c glad.h khrplatform.h
	@command -v wget >/dev/null 2>&1 || (echo "Error: wget not installed!"; exit 1)
	@wget -q -O glad.zip $(GLAD_ZIP_URL) || (echo "Error: wget download failed"; exit 1)
	@command -v unzip >/dev/null 2>&1 || (echo "Error: unzip not installed!"; exit 1)
	@unzip -q -o glad.zip || (echo "Error: unzip failed"; exit 1)
	@echo "Running GLAD generator..."
	@test -f $(GLAD_GENERATOR) || (echo "Error: generator script not found!"; exit 1)
	@chmod +x $(GLAD_GENERATOR)
	@cd $(GLAD_DIR) && ./utility/generateall.sh c || (echo "Error: GLAD generation failed"; exit 1)
	@echo "Copying generated files to project root..."
	@test -f $(GLAD_DIR)/include/glad/glad.h || (echo "Error: glad.h not generated"; exit 1)
	@cp -f $(GLAD_DIR)/include/glad/glad.h .
	@test -f $(GLAD_DIR)/include/KHR/khrplatform.h || (echo "Error: khrplatform.h not generated"; exit 1)
	@cp -f $(GLAD_DIR)/include/KHR/khrplatform.h .
	@test -f $(GLAD_DIR)/src/glad.c || (echo "Error: glad.c not generated"; exit 1)
	@cp -f $(GLAD_DIR)/src/glad.c .
	@rm -rf $(GLAD_DIR) glad.zip
	@echo "GLAD setup complete."

# Build target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(shell pkg-config --static --libs glfw3)

# Compile C++ files
main.o: main.cpp glad.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o $(shell pkg-config --cflags glfw3)

# Compile C files
glad.o: glad.c glad.h khrplatform.h
	$(CXX) $(CXXFLAGS) -c glad.c -o glad.o $(shell pkg-config --cflags glfw3)

clean:
	rm -f $(OBJECTS) $(TARGET) glad.c glad.h khrplatform.h
