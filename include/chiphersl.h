#ifndef SECL_H
#define SECL_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

long int vernam_encode(char* in);
long int vernam_decode(char* in);
int test_prime_too_num(long int p, long int e);
long int generate_prime_too_number(long int e, long int min, long int max);
int rsa_generate();
long int rsa_encode(char* in);
long int rsa_decode(char* in);
void closefiles(int N, ...);
int el_gamal_generate();
int el_gamal_encode();
int el_gamal_decode();
int shamir_generate();
int shamir_cipher();
int fsqrt_mod(long int p);

#endif
