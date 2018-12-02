#include "../include/main.h"

void bank_startup()
{
  unsigned long long int Phi, euclid_res;
  do {
    BANK.P = generate_prime_number(1, MAXINT);
    BANK.Q = generate_prime_number(1, MAXINT);
  } while (BANK.P == BANK.Q);
  BANK.N = BANK.P * BANK.Q;
  Phi = (BANK.P - 1) * (BANK.Q - 1);

  do {
    BANK.c = generate_prime_number(1, Phi);
    equlid(abs(BANK.c), abs(Phi), &euclid_res, NULL, NULL);
    BANK.d = euclid_res;
  } while (BANK.d > 0xFFFFFF);

  printf("P = %lu\tQ = %lu\tN = %lu\nC = %lu\tD = %lu\n", BANK.P, BANK.Q, BANK.N, BANK.c, BANK.d);
}

int oneside_encryption()
{
  char* res;
  char arr[1];
  unsigned long int hash1 = 0;
  hash(arr, &res);
  for (int i = 0; i < 32; ++i)
    hash1 += res[i];
  return hash1;
}

unsigned long int bank_signing(unsigned long int n1) {
  unsigned long int s1;
  s1 = mod_pow(n1, BANK.c, BANK.N);
  return s1;
}

int initialization_transaction(int *account_state, int payment_amount)
{
  unsigned long int pseudo_banknote, pseudo_sign, encrypted_banknote, r, r_rev;
  unsigned long long int euclid_res;

  do {
    EMONEY.banknote = generate_prime_number(2, BANK.N - 1);
    r = generate_prime_too_number(BANK.N, 1, BANK.N - 1);
    equlid(r, BANK.N, &euclid_res, NULL, NULL);
    r_rev = euclid_res;
  } while (r_rev > 0xFFFFFF);

  encrypted_banknote = mod_pow(r, BANK.d, BANK.N);
  pseudo_banknote = (EMONEY.banknote * encrypted_banknote) % BANK.N;
  EMONEY.banksign = bank_signing(oneside_encryption(pseudo_banknote));

  pseudo_sign = mod_pow(EMONEY.banksign, BANK.d, BANK.N);

  if (oneside_encryption(pseudo_banknote) == pseudo_sign) {
    *account_state -= payment_amount;
    if (*account_state >= 0) {
      return 0;
    } else return 1;
  } else {
    printf("%s[BANK]%s Wrong banknote sign!\n", RED, RESET);
    return 1;
  }
}
