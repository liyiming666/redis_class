CXX=g++
SRC=src
BIN=bin
BUILD=build
PROJ=redis_client
OBJS=redis_client.o
SOURCE=$(wildcard $(SRC)/*.cpp)
$(BIN)/$(PROJ):$(BUILD)/$(OBJS)
		@mkdir -p bin
		$(CXX) -std=c++11 $^ -L/usr/local/lib -lhiredis -lpthread -o  $@
$(BUILD)/$(OBJS):$(SOURCE)
		@mkdir -p build
		$(CXX) -std=c++11 -c $^ -L/usr/local/lib -lhiredis -lpthread -o $@

clean:
		rm -rf $(BIN)/* $(BUILD)/*
