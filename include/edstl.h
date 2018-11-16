#ifndef EDS_H
#define EDS_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define HASHTAB_SIZE 8192
#define HASHTAB_MUL 1023

unsigned int hashtab_hash(char *key);
int eds_rsa(char* in);
int eds_rsa_check(char* in,char* inm);
int eds_elgamal(char* in);
int eds_elgamal_check(char* in,char* inm);
int eds_gost(char* in);
int eds_gost_check(char* in,char* inm);
void hash(char *input, char **output);

#endif
