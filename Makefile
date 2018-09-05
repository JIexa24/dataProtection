CC=gcc
CFLAGS=-O0 -g3
LIB=-lcryptfl -lchiphersl -lm
LIBSRC=-L./lib/

all:MAIN

MAIN: FLAB SLAB ./obj/main.o
	$(CC) ./obj/main.o -o ./bin/main  $(LIB) $(LIBSRC)

FLAB: ./obj/firstlab.o
	ar rvs ./lib/libcryptfl.a ./obj/firstlab.o 

SLAB: ./obj/secondlab.o
	ar rvs ./lib/libchiphersl.a ./obj/secondlab.o

./obj/main.o: ./src/main.c
	$(CC) -c ./src/main.c -o ./obj/main.o

./obj/firstlab.o: ./src/firstlab.c
	$(CC) -c ./src/firstlab.c -o ./obj/firstlab.o 

./obj/secondlab.o: ./src/secondlab.c
	$(CC) -c ./src/secondlab.c -o ./obj/secondlab.o

clean:
	rm -rf ./obj/*

restruct: clean all
