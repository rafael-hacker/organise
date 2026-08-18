CXX = g++
CXXFLAGS = -std=c++17 -Iinclude
SRCS = src/main.cpp src/config.cpp src/organiser.cpp
BIN = bin/org
PREFIX = /usr/local

all:
	@mkdir -p bin/
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(BIN)

install: all
	@install -d $(PREFIX)/bin
	@install -m 755 $(BIN) $(PREFIX)/bin/org
	@echo "Installed successfully in $(PREFIX)/bin/org"

clean:
	@rm -rf bin/
