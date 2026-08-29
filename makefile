CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
INSTANCE ?= a36.txt
ITERATIONS ?= 1000

.PHONY: all build run clean

all: build

build: MCPwB

MCPwB: MCPwB.cpp
	$(CXX) $(CXXFLAGS) MCPwB.cpp -o MCPwB

run: MCPwB
	./MCPwB Instancias-20210102/$(INSTANCE) $(ITERATIONS)

clean:
	rm -f MCPwB
