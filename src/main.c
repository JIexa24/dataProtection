#include "../include/main.h"

int main (int argc, char** argv) {
  long int x, y, nod;
  long int Ka, Kb;

  equlid(11, 7, &x, &y,&nod);
  key_diffyhellman(&Ka, &Kb);

  printf("pow %ld\n", mod_pow(175,235,257));
  printf("x %ld, y %ld, nod %ld\n", x, y, nod);
  if (Ka == Kb) {
    printf("Ka %ld = %ld Kb\n", Ka, Kb);
  }

  printf("vernam_encode return %ld\n", vernam_encode("./vernam.txt", "./vernam_code.txt", "./vernam_key.txt"));
  printf("vernam_decode return %ld\n", vernam_decode("./vernam_code.txt", "./vernam_res.txt", "./vernam_key.txt"));

  rsa_generate();
  printf("rsa_encode return %ld\n", rsa_encode("./rsa.txt", "./rsa_code.txt"));
  printf("rsa_decode return %ld\n", rsa_decode("./rsa_code.txt", "./rsa_res.txt"));

  return 0;
}
