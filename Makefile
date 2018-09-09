CC=gcc
CFLAGS=-O0 -g3
LIB=-lcryptfl -lchiphersl -lm
LIBSRC=-L./lib/

all:MAIN

MAIN: FLAB SLAB ./obj/main.o
	$(CC) ./obj/main.o -o ./bin/main  $(LIB) $(LIBSRC)

FLAB: ./obj/cryptfl.o
	ar rvs ./lib/libcryptfl.a ./obj/cryptfl.o

SLAB: ./obj/chiphersl.o
	ar rvs ./lib/libchiphersl.a ./obj/chiphersl.o

./obj/main.o: ./src/main.c
	$(CC) -c ./src/main.c -o ./obj/main.o

./obj/cryptfl.o: ./src/cryptfl.c
	$(CC) -c ./src/cryptfl.c -o ./obj/cryptfl.o $(CFLAGS)

./obj/chiphersl.o: ./src/chiphersl.c
	$(CC) -c ./src/chiphersl.c -o ./obj/chiphersl.o $(CFLAGS)

clean:
	rm -rf ./obj/*

restruct: clean all
