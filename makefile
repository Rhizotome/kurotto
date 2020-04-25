CC = g++
CFLAGS = -Wall -Wextra -std=c++20 -O3 -pthread
EXEC_NAME = solveur
INCLUDES = -I /usr/include/python3.8
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lpthread
SRC = $(wildcard *.cpp) 
OBJ := $(SRC:%.cpp=%.o)

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ) callgrind*

$(EXEC_NAME) : $(OBJ)
	$(CC) -o $(EXEC_NAME) $(OBJ) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

exec: $(EXEC_NAME)
	./$(EXEC_NAME)

dll:
	$(CC) -shared -o grille_mt.so -l:libboost_python38.a -l:libboost_system.a -I/usr/include/python3.8 formule.cpp formule.hpp grillewrapper.cc grille.cpp -std=c++20 -fPIC -lpthread -O3 -Wno-undef
