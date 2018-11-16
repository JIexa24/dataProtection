#ifndef MENTAL_H
#define MENTAL_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define NUMBER_CARDS 52

struct deck {
    char suit[10];
    char name[10];
    int start_card;
};

void swap_card(unsigned long int *card_a, unsigned long int *card_b);
int mental(int n_players);

#endif
