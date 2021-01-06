CC = g++

all: MCPwB
	./MCPwB Instancias-20210102/eil76.txt 1000
	

MCPwB: MCPwB.cpp
	$(CC) MCPwB.cpp -o MCPwB
