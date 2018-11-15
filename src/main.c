#include "../include/main.h"
#include <stdint.h>
int main (int argc, char** argv) {
  srand(time(NULL));
  printf("RSA\n");
  eds_rsa("./rsa.txt");
  eds_rsa_check("./rsa.txt.rsgn", "./rsa.txt");
  printf("Elgamal\n");
  eds_elgamal("./elgamal.txt");
  eds_elgamal_check("./elgamal.txt.esgn", "./elgamal.txt");
// eds_elgamal("./elgamal.txt");
  return 0;
}
