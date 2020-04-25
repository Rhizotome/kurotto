CC = g++
CFLAGS = -Wall -Wextra -std=c++20 -O3 -pthread
EXEC_NAME = Kurotto
INCLUDES =
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lpthread -Wl,-rpath,.
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
	$(CC) -shared -o grille_mt.so -lboost_system -lboost_python38 -I/usr/include/python3.8 formule.cpp formule.hpp grillewrapper.cc grille.cpp -std=c++20 -fPIC -lpthread -O3 -Wno-undef -Wl,-rpath,.
