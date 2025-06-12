CC = gcc
CFLAGS = -Wall -std=c11 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

SRC = main.c mandelbrot.c render.c
OBJ = $(SRC:.c=.o)
BIN = mandelbrot

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BIN) *.o
