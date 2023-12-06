CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -pthread -lrt

all: mozos cocineros repostero

mozos: mozos.c
	$(CC) $(CFLAGS) -o mozos mozos.c $(LDFLAGS)

cocineros: cocineros.c
	$(CC) $(CFLAGS) -o cocineros cocineros.c $(LDFLAGS)

repostero: repostero.c
	$(CC) $(CFLAGS) -o repostero repostero.c $(LDFLAGS)

clean:
	rm -f mozos cocineros repostero
