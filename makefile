CC = gcc
CFLAGS = -Wall

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main

run: all
	./main

clean:
	rm -f main cocineros mozos repostero