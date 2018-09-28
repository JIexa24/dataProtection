#include "../include/main.h"
#include <stdint.h>
int main (int argc, char** argv) {
  long int x = 311;

  printf("fsqrt %ld = %ld\n",x, fsqrt_mod(x));
  printf("\n");
  printf("vernam_encode return %ld\n", vernam_encode("./vernam.txt", "./vernam_code.txt", "./vernam_key.txt"));
  printf("vernam_decode return %ld\n", vernam_decode("./vernam_code.txt", "./vernam_res.txt", "./vernam_key.txt"));

  printf("\n");
  rsa_generate();
  printf("rsa_encode return %ld\n", rsa_encode("./rsa.txt", "./rsa_code.txt"));
  printf("rsa_decode return %ld\n", rsa_decode("./rsa_code.txt", "./rsa_res.txt"));

  return 0;
}
