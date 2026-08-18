CXX = g++
ASFLAGS = -std=c++17
SRC = src/main.cpp
BIN = bin/org
PREFIX=/usr/local

all:
	@mkdir -p bin/
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)
	@clear

# sudo needed
install: all
	@install -d $(PREFIX)/bin
	@install -m 755 $(BIN) $(PREFIX)/bin/org
	@echo "Instalado com sucesso em $(PREFIX)/bin/org"

clean:
	@rm -rf bin/
	@clear
