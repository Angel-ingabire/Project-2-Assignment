CC = gcc
CFLAGS = -std=c11 -O2

all: bellman_ford

bellman_ford:
	$(CC) $(CFLAGS) "question 5/bellman_ford.c" -o question5_bf.exe

clean:
	-@del question5_bf.exe 2>nul || true
