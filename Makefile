CXX = g++
ASFLAGS = -std=c++17
SRC = src/main.cpp
BIN = bin/org


all:
	@mkdir -p bin/
	@mkdir -p bin/config
	@touch bin/config/config.json
	$(CXX) $(ASFLAGS) $(SRC) -o $(BIN)
	@clear
clean:
	@rm -rf bin/
	@clear
	
