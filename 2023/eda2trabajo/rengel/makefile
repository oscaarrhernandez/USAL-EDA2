CC = gcc
CFLAGS = -Wall -Wextra

all: prueba

prueba: prueba.o dispersion.o
	$(CC) $(CFLAGS) $^ -o $@

prueba.o: prueba.c dispersion.h
	$(CC) $(CFLAGS) -c $< -o $@

dispersion.o: dispersion.c dispersion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o
