CC = g++
CFLAGS = -Wall -Wextra -std=c++20 -O3 -pthread -Wno-narrowing
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

