CXX = g++
TARGET = bouncing_ball
SOURCES = main.cpp glad.c
OBJECTS = main.o glad.o

# Local paths
GLAD_ZIP = glad.zip
GLAD_TMP = glad_tmp
LOCAL_INCLUDE = local_include
LOCAL_SRC = local_src
PKGCONFIG_DIR = pkgconfig
PKGCONFIG_FILE = $(PKGCONFIG_DIR)/glad.pc

# Compiler flags via pkg-config
CXXFLAGS = -std=c++17 -O2 $(shell PKG_CONFIG_PATH=$(PKGCONFIG_DIR) pkg-config --cflags glad glfw3)
LDFLAGS  = $(shell pkg-config --static --libs glfw3) \
           -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

all: brew glfw glad $(TARGET)

# Install Homebrew if missing (macOS only)
brew:
	@command -v brew >/dev/null 2>&1 || ( \
		echo "Homebrew not found. Installing..."; \
		/bin/bash -c "$$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"; \
	)

# Install GLFW via Homebrew if missing
glfw:
	@brew list glfw >/dev/null 2>&1 || ( \
		echo "GLFW not found. Installing via Homebrew..."; \
		brew install glfw; \
	)

# Extract GLAD and set up local pkg-config
glad:
	@echo "==> Setting up local GLAD..."
	@test -f $(LOCAL_INCLUDE)/glad/glad.h || \
	rm -rf $(GLAD_TMP) $(LOCAL_INCLUDE) $(LOCAL_SRC) $(PKGCONFIG_DIR)
	@unzip -q -o $(GLAD_ZIP) -d $(GLAD_TMP) || (echo "Error: unzip failed"; exit 1)
	@mkdir -p $(LOCAL_INCLUDE)/glad $(LOCAL_INCLUDE)/KHR $(LOCAL_SRC) $(PKGCONFIG_DIR)
	@cp -f $(GLAD_TMP)/include/glad/glad.h $(LOCAL_INCLUDE)/glad/
	@cp -f $(GLAD_TMP)/include/KHR/khrplatform.h $(LOCAL_INCLUDE)/KHR/
	@cp -f $(GLAD_TMP)/src/glad.c $(LOCAL_SRC)/glad.c
	@echo "prefix=$(CURDIR)" > $(PKGCONFIG_FILE)
	@echo "exec_prefix=$$prefix" >> $(PKGCONFIG_FILE)
	@echo "includedir=$$prefix/$(LOCAL_INCLUDE)" >> $(PKGCONFIG_FILE)
	@echo "libdir=$$prefix/lib" >> $(PKGCONFIG_FILE)
	@echo "Name: glad" >> $(PKGCONFIG_FILE)
	@echo "Description: Local GLAD OpenGL loader" >> $(PKGCONFIG_FILE)
	@echo "Version: 2.0.8" >> $(PKGCONFIG_FILE)
	@echo "Cflags: -I$${includedir} -I$${includedir}/KHR" >> $(PKGCONFIG_FILE)
	@echo "Libs:" >> $(PKGCONFIG_FILE)
	@rm -rf $(GLAD_TMP)
	@echo "Local GLAD setup complete."

# Build target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Compile C++ files
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Compile GLAD C file
glad.o: $(LOCAL_SRC)/glad.c
	$(CXX) $(CXXFLAGS) -c $(LOCAL_SRC)/glad.c -o glad.o

clean:
	rm -rf $(OBJECTS) $(TARGET) $(LOCAL_INCLUDE) $(LOCAL_SRC) $(PKGCONFIG_DIR)
