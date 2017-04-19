CFLAGS=-g -c
OUTPUT=-o server.exe
CC=/usr/local/bin/gcc.exe

all: server.exe

server.exe: main.o
	$(CC) $(OUTPUT) main.o

main.o: main.c
	$(CC) $(CFLAGS) main.c