# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática

CXX = g++						        
CXXFLAGS = -std=c++2a -g -Wall -pedantic
LDFLAGS = # The linker options (if any)		

.PHONY: server_exec clean
	
server_exec : ServerMain.o Server/ServerTCPManager.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
clean:
	@ rm -f *.o server_exec
	@ rm -f Server/*.o

# The $@ and $< are called automatic variables. The variable $@ represents the name 
# of the target and $< represents the first prerequisite required to create the output file.