CC= gcc
CFLAGS= -Wall -std=c99 -g -I "include/"
EXEC=database
SRC= src/index.c src/directory.c src/main.c src/appli.c
OBJ= $(SRC:.c=.o)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf src/*.o

mrproper: clean
	rm -rf $(EXEC)
