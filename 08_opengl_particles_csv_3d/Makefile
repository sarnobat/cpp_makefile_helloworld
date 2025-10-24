# Compiler and flags
CXX      = c++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17
LDFLAGS  =
BIN      = main
CPPSRC   = main.cpp
ASSETS   = globe.jpg
EMBEDDED = globe_jpg.h
EXTRA_HEADERS = stb_image.h $(EMBEDDED)

# === Choose your toolkit ===
# For classic GLUT code:
USE_GLUT = 0
# For modern GLFW + GLEW code:
# USE_GLUT = 0

# === Homebrew fallback paths (for macOS) ===
BREW_PREFIX = $(shell brew --prefix)
GLEW_INCLUDE = $(BREW_PREFIX)/include
GLEW_LIB     = $(BREW_PREFIX)/lib
GLFW_INCLUDE = $(BREW_PREFIX)/include
GLFW_LIB     = $(BREW_PREFIX)/lib

# === Flags setup ===
ifeq ($(USE_GLUT),1)
    ifneq ($(shell pkg-config --exists glew && echo yes),yes)
        CXXFLAGS += -I$(GLEW_INCLUDE)
        LDFLAGS  += -L$(GLEW_LIB) -lGLEW -framework GLUT -framework OpenGL
    else
        CXXFLAGS += $(shell pkg-config --cflags glew)
        LDFLAGS  += $(shell pkg-config --libs glew) -framework GLUT -framework OpenGL
    endif
else
    ifneq ($(shell pkg-config --exists glew glfw3 && echo yes),yes)
        CXXFLAGS += -I$(GLEW_INCLUDE) -I$(GLFW_INCLUDE)
        LDFLAGS  += -L$(GLEW_LIB) -lGLEW -L$(GLFW_LIB) -lglfw -framework OpenGL
    else
        CXXFLAGS += $(shell pkg-config --cflags glew glfw3)
        LDFLAGS  += $(shell pkg-config --libs glew glfw3) -framework OpenGL
    endif
endif

# === Asset embedding ===
# Automatically convert globe.jpg → globe_jpg.h for static inclusion
$(EMBEDDED): $(ASSETS)
	@echo "Embedding $< → $@"
	xxd -i $< > $@

# === stb_image header (self-contained single-file library) ===
stb_image.h:
	@echo "Fetching stb_image.h..."
	curl -s -L -o stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h

# === Build target ===
$(BIN): $(CPPSRC) $(EXTRA_HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# === Run target ===
run: $(BIN)
	./$(BIN) data.csv &
	@echo "Try running on /Volumes/git/github/depot_parent/rust/20_exif_to_csv/out.csv, i.e."
	@echo "./$(BIN) --map exif.csv"

# === Clean target ===
clean:
	rm -f $(BIN) $(EMBEDDED)

# === Static build prerequisites ===
staticinit:
	@echo "Fetching stb_image.h..."
	curl -s -L -o stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
	@echo "Embedding globe.jpg as C array..."
	xxd -i globe.jpg > globe_jpg.h
	@echo "Done. Ready for static build."

# === Static build target ===
static: staticinit
	$(CXX) $(CXXFLAGS) -o $(BIN) $(CPPSRC) \
		-I$(GLEW_INCLUDE) -I$(GLFW_INCLUDE) -I. \
		-L$(GLEW_LIB) -lGLEW -L$(GLFW_LIB) -lglfw \
		-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

.PHONY: clean run static
