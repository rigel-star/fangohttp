CC = gcc
SRC = main.c ./src/http/*.c ./src/utils/file_util.c
CFLAGS = -g -Wall -Wextra

INCS = ./lib/dds/include
LIBS = ./lib/dds/lib

ELF = main

all: $(ELF)

$(ELF): $(SRC)
	$(CC) -I$(INCS)/ $(CFLAGS) $^ -o $@ -L$(LIBS)/ -lhashtab -llist

clean:
	rm main
