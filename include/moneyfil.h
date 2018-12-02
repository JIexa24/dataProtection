#ifndef FIL_H
#define FIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

#define BLUE    "\033[1;94m"
#define YELLOW  "\033[1;93m"
#define GREEN   "\033[1;92m"
#define RED     "\033[1;91m"
#define BLACK   "\033[1;90m"
#define WHITE   "\033[1;89m"
#define RESET   "\033[0m"

struct data_BANK {
    unsigned long int P;
    unsigned long int Q;
    unsigned long int N;
    unsigned long int c;
    unsigned long int d;
} BANK;

struct electronic_money {
  unsigned long int banknote;
  unsigned long int banksign;
} EMONEY;

void bank_startup();
int oneside_encryption();
int initialization_transaction(int *account_state, int payment_amount);

#endif
