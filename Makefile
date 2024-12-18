CXX := g++ -std=c++20
CXXFLAGS := -g3 -Wall

SRC_FILES := $(wildcard src/*.cpp)
INC_DIR := ./inc

BUILD_DIR = ./build/

all:
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -I$(INC_DIR) -o $(BUILD_DIR)lcc