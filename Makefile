CC = gcc
EXEC = blockchain
SRC = $(wildcard *.c)
OBJ = $(SRC: .c = .o)

all : $(EXEC)

%o : %c
	$(CC) -o $(EXEC) -c $<

$(EXEC) : $(OBJ)
	$(CC) -o $(EXEC) $^
