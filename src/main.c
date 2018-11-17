#include "../include/main.h"
#include <stdint.h>
int main (int argc, char** argv) {
  srand(time(NULL));

  int n_players;
  n_players = atoi(argv[1]);
  mental(n_players);
// eds_elgamal("./elgamal.txt");
  return 0;
}
