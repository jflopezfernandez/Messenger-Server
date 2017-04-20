CFLAGS=-g -c
OUTPUT=-o server.exe
CC=/usr/local/bin/gcc.exe

all: server.exe

server.exe: util.o tcpserverutility.o main.o
	$(CC) $(OUTPUT) util.o tcpserverutility.o main.o

main.o: main.c
	$(CC) $(CFLAGS) main.c

tcpserverutility.o: tcpserverutility.c
	$(CC) $(CFLAGS) tcpserverutility.c

util.o: util.c
	$(CC) $(CFLAGS) util.c