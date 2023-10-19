CC=gcc
CFLAGS=-I. # для подключения библиотек

objects = fact deadlock

all: $(objects)

fact : ../../lab3/src/utils.h utils.o
	$(CC) -o fact utils.o fact.c -pthread $(CFLAGS)

deadlock: 
	$(CC) -o deadlock deadlock.c -pthread $(CFALGS)

utils.o : ../../lab3/src/utils.h
	$(CC) -o utils.o -c ../../lab3/src/utils.c $(CFLAGS)

clean :
	rm utils.o fact deadlock mutex
