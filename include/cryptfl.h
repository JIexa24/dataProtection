#ifndef FL_H
#define FL_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define INT32_MAX 2147483647

unsigned long long int mod_pow(unsigned long long int num, unsigned long long int pow,unsigned long long int mod);
void equlid(long long int a, long long int b, long long int* x, long long int *y, long long int* nod);
void key_diffyhellman();
long long int gcd(long long int a, long long int b);
int test_prime_num(long long int p);
long long int generate_prime_number(long long int min, long long int max);
long long int small_big_steps(long long int g, long long int a, long long int p) ;

#endif
