CC=gcc
CFLAGS=-O0 -g3
LIB=-lchiphersl -lcryptfl -ledstl -lm
LIBSRC=-L./lib/
BIN=./bin/main

all:MAIN

run: MAIN
	$(BIN)

MAIN: FLAB SLAB TLAB ./obj/main.o
	$(CC) ./obj/main.o -o $(BIN)  $(LIB) $(LIBSRC)

FLAB: ./obj/cryptfl.o
	ar rvs ./lib/libcryptfl.a ./obj/cryptfl.o

SLAB: ./obj/chiphersl.o
	ar rvs ./lib/libchiphersl.a ./obj/chiphersl.o

TLAB: ./obj/edstl.o
	ar rvs ./lib/libedstl.a ./obj/edstl.o

./obj/main.o: ./src/main.c
	$(CC) -c ./src/main.c -o ./obj/main.o

./obj/edstl.o: ./src/edstl.c
	$(CC) -c ./src/edstl.c -o ./obj/edstl.o $(CFLAGS)

./obj/cryptfl.o: ./src/cryptfl.c
	$(CC) -c ./src/cryptfl.c -o ./obj/cryptfl.o $(CFLAGS)

./obj/chiphersl.o: ./src/chiphersl.c
	$(CC) -c ./src/chiphersl.c -o ./obj/chiphersl.o $(CFLAGS)

clean:
	rm -rf ./obj/*

restruct: clean all
