CC= gcc
CFLAGS= -Wall -std=c99 -g -I "include/"
EXEC=database
SRC= src/index.c src/directory.c src/main.c
OBJ= $(SRC:.c=.o)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@


clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)