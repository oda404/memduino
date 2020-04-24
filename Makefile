CC=gcc
CFLAGS=-lm

make: main.o serialW.o
	$(CC) main.o serialW.o -o memDuino.out $(CFLAGS)
	rm *.o

main.o:
	$(CC) -c src/c/main.c

serialW.o:
	$(CC) -c src/c/serialW.c
