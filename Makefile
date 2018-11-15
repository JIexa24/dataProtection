CC=gcc
CFLAGS=-O0 -g3
LIB= -ledstl -lcryptfl -lchiphersl -lm
LIBSRC=-L./lib/
BIN=./bin/main

all: MAIN

run: MAIN
	$(BIN)

MAIN: FLAB SLAB TLAB ./obj/main.o
	$(CC) ./obj/main.o -o $(BIN) $(LIB) $(LIBSRC)

FLAB: ./obj/cryptfl.o
	ar rvs ./lib/libcryptfl.a ./obj/cryptfl.o

SLAB: ./obj/chiphersl.o
	ar rvs ./lib/libchiphersl.a ./obj/chiphersl.o

TLAB: ./obj/edstl.o ./obj/md5.o
	ar rvs ./lib/libedstl.a ./obj/edstl.o ./obj/md5.o

./obj/main.o: ./src/main.c
	$(CC) -c ./src/main.c -o ./obj/main.o $(CFLAGS)

./obj/md5.o: ./src/md5.c
	$(CC) -c ./src/md5.c -o ./obj/md5.o $(CFLAGS)

./obj/edstl.o: ./src/edstl.c
	$(CC) -c ./src/edstl.c -o ./obj/edstl.o $(CFLAGS)

./obj/cryptfl.o: ./src/cryptfl.c
	$(CC) -c ./src/cryptfl.c -o ./obj/cryptfl.o $(CFLAGS)

./obj/chiphersl.o: ./src/chiphersl.c
	$(CC) -c ./src/chiphersl.c -o ./obj/chiphersl.o $(CFLAGS)

clean:
	rm -rf ./lib/*.a
	rm -rf ./obj/*.o

restruct: clean all
