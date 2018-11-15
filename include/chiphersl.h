#ifndef SECL_H
#define SECL_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

long long int vernam_encode(char* in);
long long int vernam_decode(char* in);
int test_prime_too_num(long long int p, long long int e);
long long int generate_prime_too_number(long long int e, long long int min, long long int max);
int rsa_generate();
long long int rsa_encode(char* in);
long long int rsa_decode(char* in);
void closefiles(int N, ...);
int elgamal_generate();
long long int elgamal_encode(char* in);
long long int elgamal_decode(char* in);
void shamir_generate(unsigned long long int* p, unsigned long long int* c, unsigned long long int* d);
int shamir_cipher(char* in);
int fsqrt_mod(long long int p);

#endif
