CC=gcc
CFLAGS=-I. # для подключения библиотек

objects = process_memory parallel psum

all: $(objects)

process_memory : process_memory.c
	$(CC) -o process process_memory.c $(CFLAGS)

parallel : utils.o find_min_max.o ../../lab3/src/utils.h ../../lab3/src/find_min_max.h
	$(CC) -o parallel utils.o find_min_max.o ../../lab3/src/parallel_min_max.c $(CFLAGS)

psum : ../../lab3/src/utils.h utils.o
	$(CC) -o psum utils.o parallel_sum.c -pthread $(CFLAGS)

utils.o : ../../lab3/src/utils.h
	$(CC) -o utils.o -c ../../lab3/src/utils.c $(CFLAGS)

find_min_max.o : ../../lab3/src/utils.h ../../lab3/src/find_min_max.h
	$(CC) -o find_min_max.o -c ../../lab3/src/find_min_max.c $(CFLAGS)

clean :
	rm process find_min_max.o utils.o psum parallel
