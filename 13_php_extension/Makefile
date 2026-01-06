PHP ?= php
PHP_CONFIG ?= php-config
CXX ?= c++

EXT_NAME := helloworld
EXT_DIR := modules
EXT_SO := $(CURDIR)/$(EXT_DIR)/$(EXT_NAME).so

CXXFLAGS := -std=c++11 -fPIC -shared $(shell $(PHP_CONFIG) --includes) -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1
LDFLAGS := $(shell $(PHP_CONFIG) --ldflags) $(shell $(PHP_CONFIG) --libs) -Wl,-undefined,dynamic_lookup

SERVER_HOST := 127.0.0.1
SERVER_PORT := 8000
SERVER_URL := http://$(SERVER_HOST):$(SERVER_PORT)

.PHONY: all build install serve open run clean

all: run

$(EXT_DIR):
	mkdir -p $(EXT_DIR)

build: $(EXT_DIR)
	$(CXX) $(CXXFLAGS) -o $(EXT_SO) helloworld.cpp $(LDFLAGS)

install: build
	@echo "Installing $(EXT_NAME).so to $$( $(PHP_CONFIG) --extension-dir )"
	cp $(EXT_SO) "$$( $(PHP_CONFIG) --extension-dir )/$(EXT_NAME).so"

serve: build
	$(PHP) -d extension=$(EXT_SO) -S $(SERVER_HOST):$(SERVER_PORT) -t public

open:
	open $(SERVER_URL)

run: build
	@$(PHP) -d extension=$(EXT_SO) -S $(SERVER_HOST):$(SERVER_PORT) -t public & \
	sleep 1; \
	open $(SERVER_URL); \
	wait

clean:
	rm -rf $(EXT_DIR)/*.so
