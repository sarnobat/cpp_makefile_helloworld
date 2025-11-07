# ========================================
# Find My Friends - Redis Pub/Sub Simulator
# ========================================

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
SRC = src
BIN = bin
LOGDIR = logs

# =====================
# vcpkg / cpp-redis setup
# =====================
VCPKG_ROOT ?= $(HOME)/vcpkg
VCPKG = $(VCPKG_ROOT)/vcpkg

UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),arm64)
	VCPKG_TRIPLET = arm64-osx
else
	VCPKG_TRIPLET = x64-osx
endif

CXXFLAGS += -I$(VCPKG_ROOT)/installed/$(VCPKG_TRIPLET)/include
LDFLAGS  += -L$(VCPKG_ROOT)/installed/$(VCPKG_TRIPLET)/lib
LIBS     = -lcpp_redis -ltacopie

# ---------------------
# Info / run commands (order-independent)
# ---------------------
info:

	@echo "========================================="
	@echo "Find My Friends Simulation - Quick Start"
	@echo "========================================="
	@echo ""
	@echo "make run-server-ingest    make run-server-forward  make run-receiver      make run-clients
	@echo "+-------------------+     +-------------------+    +--------------+     +---------------------------+
	@echo "| server_ingest.cpp | --> | server_forward.cpp| => | receiver.cpp | --> |  client.cpp (client0..4)  |
	@echo "+-------------------+     +-------------------+    +--------------+     +---------------------------+
	@echo ""
	@echo "Run the simulation using the following commands in ANY order:"
	@echo ""
	@echo "  make run-server-ingest       # logs to console"
	@echo "  make run-server-forward      # logs to console"
	@echo "  make run-receiver            # logs to console"
	@echo "  make run-clients             # logs to logs/client0.log .. logs/client4.log"
	@echo ""
	@echo "iTerm2 tips for multiple panes:"
	@echo "  1) Split horizontally: Cmd + D"
	@echo "  2) Split vertically:   Cmd + Shift + D"
	@echo "  3) Broadcast input to all panes in tab: Cmd + Option + I (and again to turn off)"
	@echo ""
	@echo "Alternatively, use this one-click launcher:"
	@echo ""
	@echo "sh iterm_4panes.sh"
	@echo ""
	@echo "========================================="

# =====================
# Main targets
# =====================
all: deps dirs client server_ingest server_forward receiver

dirs:
	mkdir -p $(BIN) $(LOGDIR)

# ---------------------
# Install dependencies
# ---------------------
deps:
	@echo "Installing dependencies..."
	# Redis
	brew list redis >/dev/null 2>&1 || brew install redis
	# Clone vcpkg if missing
	test -d $(VCPKG_ROOT) || git clone https://github.com/Microsoft/vcpkg $(VCPKG_ROOT)
	# Bootstrap vcpkg
	$(VCPKG) --version >/dev/null 2>&1 || $(VCPKG_ROOT)/bootstrap-vcpkg.sh
	# Install cpp-redis
	$(VCPKG) list | grep -q cpp-redis || $(VCPKG) install cpp-redis:$(VCPKG_TRIPLET)
	@echo "Dependencies installed."

# ---------------------
# Start Redis via Homebrew
# ---------------------
start-redis:
	@brew services list | grep redis | grep started >/dev/null 2>&1 || \
		(brew services start redis && echo "Redis started.")
	@echo "Redis should now be running. Test with 'redis-cli ping'."

# ---------------------
# Compile targets
# ---------------------
client:
	$(CXX) $(CXXFLAGS) -o $(BIN)/client $(SRC)/client.cpp $(LDFLAGS) $(LIBS)

server_ingest:
	$(CXX) $(CXXFLAGS) -o $(BIN)/server_ingest $(SRC)/server_ingest.cpp $(LDFLAGS) $(LIBS)

server_forward:
	$(CXX) $(CXXFLAGS) -o $(BIN)/server_forward $(SRC)/server_forward.cpp $(LDFLAGS) $(LIBS)

receiver:
	$(CXX) $(CXXFLAGS) -o $(BIN)/receiver $(SRC)/receiver.cpp $(LDFLAGS) $(LIBS)

# ---------------------
# Run targets
# ---------------------
run-clients: start-redis client
	@echo "Starting 5 clients (background). Logs -> $(LOGDIR)/clientN.log"
	nohup stdbuf -oL -eL $(BIN)/client 0 &> $(LOGDIR)/client0.log &
	sleep 0.2
	nohup stdbuf -oL -eL $(BIN)/client 1 &> $(LOGDIR)/client1.log &
	sleep 0.2
	nohup stdbuf -oL -eL $(BIN)/client 2 &> $(LOGDIR)/client2.log &
	sleep 0.2
	nohup stdbuf -oL -eL $(BIN)/client 3 &> $(LOGDIR)/client3.log &
	sleep 0.2
	nohup stdbuf -oL -eL $(BIN)/client 4 &> $(LOGDIR)/client4.log &
	@echo "Clients started."

run-server-ingest: start-redis server_ingest
	@echo "Starting server_ingest (foreground). Ctrl-C to stop."
	$(BIN)/server_ingest

run-server-forward: start-redis server_forward
	@echo "Starting server_forward (foreground). Ctrl-C to stop."
	$(BIN)/server_forward

run-receiver: start-redis receiver
	@echo "Starting receiver (foreground). Ctrl-C to stop."
	$(BIN)/receiver

# ---------------------
# Stop background processes
# ---------------------
stop:
	-pkill -f $(BIN)/client || true
	-pkill -f $(BIN)/server_ingest || true
	-pkill -f $(BIN)/server_forward || true
	-pkill -f $(BIN)/receiver || true
	@echo "Stopped processes (if any)."

# ---------------------
# Clean
# ---------------------
clean: stop
	rm -rf $(BIN) $(LOGDIR)
	@echo "Cleaned bins and logs."

.PHONY: all deps dirs start-redis client server_ingest server_forward receiver \
        run-clients run-server-ingest run-server-forward run-receiver stop clean
