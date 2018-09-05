#ifndef SECL_H
#define SECL_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>

long int vernam_encode(char* in, char* out, char* key);
long int vernam_decode(char* in, char* out, char* key);
int test_prime_too_num(long int p, long int e);
long int generate_prime_too_number(long int e);
void rsa_generate();
void rsa_encode(char* in, char* out);
void rsa_decode(char* in, char* out);

#endif
