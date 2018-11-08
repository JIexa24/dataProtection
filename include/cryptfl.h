#ifndef FL_H
#define FL_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define INT32_MAX 2147483647

unsigned long int mod_pow(unsigned long int num, unsigned long int pow,unsigned long int mod);
void equlid(long int a, long int b, long int* x, long int *y, long int* nod);
void key_diffyhellman();
long int gcd(long int a, long int b);
int test_prime_num(long int p);
long int generate_prime_number(long int min, long int max);
long int small_big_steps(long int g, long int a, long int p) ;

#endif
