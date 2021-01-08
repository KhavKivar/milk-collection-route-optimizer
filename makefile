CC = g++

all: MCPwB
	./MCPwB Instancias-20210102/$(ARGS) 
	

MCPwB: MCPwB.cpp
	$(CC) MCPwB.cpp -o MCPwB
