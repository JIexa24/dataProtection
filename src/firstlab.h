#ifndef FL_H
#define FL_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long int fast_pow(long int num, unsigned long int pow,unsigned long int mod);
void equlid(long int a, long int b, long int* x, long int *y, long int* nod);
void key_diffyhellman();
int test_prime_num(long int p);
long int generate_prime_number();
long int small_big_steps(long int g, long int a, long int p) ;

#endif
