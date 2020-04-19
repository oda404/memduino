CC=gcc
CFLAGS=-lm

make: main.o serialW.o
	$(CC) main.o serialW.o -o MemDuinoC.out $(CFLAGS)
	rm *.o

main.o:
	$(CC) -c -lm src/c/main.c

serialW.o:
	$(CC) -c src/c/serialW.c
