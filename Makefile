CC = gcc
SRC = main.c ./src/http/*.c ./src/utils/file_util.c
CFLAGS = -g -Wall -Wextra

ELF = main

all: $(ELF)

$(ELF): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm main
