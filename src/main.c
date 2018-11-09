#include "../include/main.h"
#include <stdint.h>
int main (int argc, char** argv) {
  srand(time(NULL));
//  eds_rsa("./elgamal.txt");
 eds_elgamal("./elgamal.txt");
  return 0;
}

 void test_chipphersl() {
   printf("vernam_encode return %ld\n", vernam_encode("./autorun.ico"));
   printf("vernam_decode return %ld\n", vernam_decode("./autorun.ico.encode"));

   printf("\n");
   rsa_generate();
   printf("rsa_encode return %ld\n", rsa_encode("./autorun1.ico"));
   printf("rsa_decode return %ld\n", rsa_decode("./autorun1.ico.encode"));
   //shamir_generate();
   printf("shamir_encode return %d\n", shamir_cipher("./autorun2.ico"));
   elgamal_generate();
   printf("elgamal_encode return %ld\n", elgamal_encode("./autorun3.ico"));
   printf("elgamal_decode return %ld\n", elgamal_decode("./autorun3.ico.encode"));

   printf("vernam_encode return %ld\n", vernam_encode("./vernam.txt"));
   printf("vernam_decode return %ld\n", vernam_decode("./vernam.txt.encode"));

   printf("\n");
   rsa_generate();
   printf("rsa_encode return %ld\n", rsa_encode("./rsa.txt"));
   printf("rsa_decode return %ld\n", rsa_decode("./rsa.txt.encode"));
   //shamir_generate();
   printf("shamir_encode return %d\n", shamir_cipher("./shamir.txt"));
   elgamal_generate();
   printf("elgamal_encode return %ld\n", elgamal_encode("./elgamal.txt"));
   printf("elgamal_decode return %ld\n", elgamal_decode("./elgamal.txt.encode"));

 }
