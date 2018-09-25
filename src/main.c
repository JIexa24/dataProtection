#include "../include/main.h"
#include <stdint.h>
int main (int argc, char** argv) {
  long int x, y, nod;
  long int Ka, Kb;

  equlid(373 , 33, &x, &y,&nod);
  key_diffyhellman(&Ka, &Kb);

  printf("pow %ld %d\n", mod_pow(809,593,1187), sizeof(long int));
  printf("x %ld, y %ld, nod %ld\n", x, y, nod);
  if (Ka == Kb) {
    printf("Ka %ld = %ld Kb\n", Ka, Kb);
  }
  printf("\n");
  printf("sbs 3,13,17 (4)= %ld\n", small_big_steps(3, 13, 17));
  printf("sbs 5,3,23 (16)= %ld\n", small_big_steps(5, 3, 23));
  printf("\n");
  printf("vernam_encode return %ld\n", vernam_encode("./vernam.txt", "./vernam_code.txt", "./vernam_key.txt"));
  printf("vernam_decode return %ld\n", vernam_decode("./vernam_code.txt", "./vernam_res.txt", "./vernam_key.txt"));

  printf("\n");
  rsa_generate();
  printf("rsa_encode return %ld\n", rsa_encode("./rsa.txt", "./rsa_code.txt"));
  printf("rsa_decode return %ld\n", rsa_decode("./rsa_code.txt", "./rsa_res.txt"));

  return 0;
}
