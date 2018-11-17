#include "../include/main.h"

void swap_card(unsigned long int *card_a, unsigned long int *card_b)
{
  unsigned long int tmp;
  tmp = *card_a;
  *card_a = *card_b;
  *card_b = tmp;
}

int mental(int n_players) {
  unsigned long int general_p;
  int cards[NUMBER_CARDS] = {0};
  general_p = generate_prime_number(1000, MAXINT);
  printf("General P was generated:\t%lu\n", general_p);

  unsigned long int c[n_players], d[n_players];
  unsigned long long int euclid_res0, euclid_res1, euclid_res2;
  for (int i = 0; i < n_players; ++i) {
    do {
      c[i] = generate_prime_too_number(general_p - 1, 1, general_p - 1);
      equlid(c[i], general_p - 1, &euclid_res0, NULL, NULL);
      d[i] = euclid_res0;
    } while (d[i] > 0xFFFFFF);
    printf("For Player %d generated:\t%lu\t%lu\n", i, c[i], d[i]);
  }

  FILE *deck_file = fopen("deck", "r");
  if (deck_file == NULL) {
      fprintf(stderr, "File doesn't exist!\n");
      exit(EXIT_FAILURE);
  }
  struct deck game_deck[NUMBER_CARDS];
  unsigned long int encoded_deck[NUMBER_CARDS];
  for(int i = 0; i < NUMBER_CARDS; ++i) {
      cards[i] = 0;
      fscanf(deck_file, "%s", game_deck[i].suit);
      fscanf(deck_file, "%s", game_deck[i].name);
      game_deck[i].start_card = i + 2;
//      printf("Read card %d %s %s\n", i + 2, game_deck[i].suit, game_deck[i].name);
      encoded_deck[i] = i + 2;
  }
  fclose(deck_file);

  for (int j = 0; j < NUMBER_CARDS; ++j) {
    for(int i = 0; i < n_players; ++i){
      encoded_deck[j] = mod_pow(encoded_deck[j], c[i], general_p);
    }
//    printf("%d encode %ld\n",game_deck[j].start_card, encoded_deck[j]);
  }

  int rand_card;
  for(int j = 0; j < NUMBER_CARDS; ++j) {
    rand_card = rand() % NUMBER_CARDS;
    swap_card(&encoded_deck[j], &encoded_deck[rand_card]);
  }

  unsigned long int curr_card = NUMBER_CARDS;
  struct deck player_hand[n_players][2];
  for (int i = 0; i < n_players; ++i) {
    int rand_card;
    for(int j = 0; j < 2; j++) {
      do {
        rand_card = rand() % NUMBER_CARDS;
      } while (cards[rand_card] == 1);
      cards[rand_card] = 1;
      printf("rand %d -> ", rand_card);
      for(int k = 0; k < n_players; k++)
        encoded_deck[rand_card] = mod_pow(encoded_deck[rand_card], d[k], general_p);
        printf("%ld\n",encoded_deck[rand_card]);
      for(int k = 0; k < NUMBER_CARDS; k++) {
        if (game_deck[k].start_card == encoded_deck[rand_card]) {
          player_hand[i][j].start_card = game_deck[k].start_card;
          strcpy(player_hand[i][j].suit, game_deck[k].suit);
          strcpy(player_hand[i][j].name, game_deck[k].name);
          k = NUMBER_CARDS;
        }
      }
      printf("\n");
    }
  }

  printf("\nCards on the board:\n");
  for(int j = 0; j < 5; j++) {
    do {
      rand_card = rand() % NUMBER_CARDS;
    } while (cards[rand_card] == 1);
    cards[rand_card] = 1;
    for(int k = 0; k < n_players; k++)
      encoded_deck[rand_card] = mod_pow(encoded_deck[rand_card], d[k], general_p);
    
    for(int k = 0; k < NUMBER_CARDS; k++) {
      if (game_deck[k].start_card == encoded_deck[rand_card]) {
        printf("%s %s\n", game_deck[k].name, game_deck[k].suit);
      }
    }
  }
  printf("\n");
  for(int i = 0; i < n_players; i++) {
    printf("Player %d:\n", i + 1);
    printf("%s %s\n", player_hand[i][0].name, player_hand[i][0].suit);
    printf("%s %s\n", player_hand[i][1].name, player_hand[i][1].suit);
    printf("\n");
  }
  printf("Widow: %lu cards\n", curr_card);

  return 0;
}
