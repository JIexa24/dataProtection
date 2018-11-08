#include "../include/main.h"

int eds_rsa(char* in) {
  int fdin;
  int fdpub, fdpriv;
  int i = 0;
  unsigned long int ki = 0, hash_i;
  unsigned long int n = 0, d = 0, e = 0;
  char c = '\0';
  char keych = '\0';
  char out[256] = {0};
  char *keystr = malloc(sizeof(char));
  unsigned long int *keystrs = malloc(sizeof(unsigned long int) * 32);
  char *hashkeystr = NULL;

  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    closefiles(1, fdin);
    return -1;
  }
  while (read(fdin, &c, sizeof(char)) != 0) {
    keystr[ki] = c;
    ++ki;
    keystr = realloc(keystr, sizeof(char) * (ki + 1));
  }
  keystr[ki] = 0;
  hash(keystr, &hashkeystr);
  rsa_generate();
  if ((fdpub = open(".keyrsa.pub", O_RDONLY)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fdin);
    return -1;
  }
  if ((fdpriv = open(".keyrsa", O_RDONLY)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(2, fdin, fdpub);
    return -1;
  }

  read(fdpub, &e, sizeof(unsigned long int));
  read(fdpriv, &d, sizeof(unsigned long int));
  read(fdpriv, &n, sizeof(unsigned long int));
  printf("%s\n", hashkeystr);
  for (i = 0; i < 32; ++i) {
    keystrs[i] = mod_pow((unsigned long int)hashkeystr[i], d, n);
  //  printf("%lu\n", keystrs[i]);
  }
  for (i = 0; i < 32; ++i) {
    hashkeystr[i] = (char)mod_pow(keystrs[i], e, n);
  }

  printf("%s\n", hashkeystr);
  closefiles(3, fdin, fdpub, fdpriv);
}

int eds_elgamal(char* in) {
  int fdin;
  unsigned long int ki = 0, hash_i;
  char c = '\0';
  char keych = '\0';
  char out[256] = {0};
  char *keystr = malloc(sizeof(char));
  char *hashkeystr = NULL;
  unsigned long int p = 23;// generate_prime_number(1, 1000);
  unsigned long int k = 5;// generate_prime_too_number(p - 1 ,1, p - 1);
  long int k1 = 0, nod = 0;
  unsigned long int g = fsqrt_mod(p);
  unsigned long int r = mod_pow(g, k, p);
  unsigned long int x = 7;//rand() % (p - 1) + 1;
  unsigned long int y = mod_pow(g, x, p);
  unsigned long int s = 0;
  unsigned long int testl = 0;
  unsigned long int testr = 0;
  do {
    equlid(k, p - 1, &k1, &testl, &nod);
    k1 = k1 % (p - 1);
  } while (nod != 1);

  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }

  while (read(fdin, &c, sizeof(char)) != 0) {
    keystr[ki] = c;
    ++ki;
    keystr = realloc(keystr, sizeof(char) * (ki + 1));
  }

  keystr[ki] = 0;

  hash(keystr, &hashkeystr);
  hash_i = hashtab_hash(hashkeystr);
  hash_i = 3;
  s = (hash_i - x * r) % (p - 1);
  s = (s * k1) % (p - 1);
  s = 21;
  testl = mod_pow(y, r, p);
  testl = testl * mod_pow(r, s, p);
  testl = testl % p;
  testr = mod_pow(g, hash_i, p);
  printf("%lu == %lu\n", testl, testr);
  if ((0 < r && r < p) && (0 < s && s < (p - 1))) {
    printf("correct\n");
  } else {
    printf("incorrect\n");
  }
  printf("%s\n%s ---- %lu\neds = %lu, %lu\n\n", keystr, hashkeystr, hash_i, r, s);
  printf("p = %lu, g = %lu, x = %lu, y = %lu, k = %lu, r = %lu, s = %lu, k1 = %lu\n\n", p, g, x, y, k, r, s, k1);
}

int eds_gost(char* in) {

}

void hash(char *input, char **output)
{
  md5_state_t HASH;
  md5_byte_t digest[16];
  char *hex_output = NULL;
  hex_output = malloc((16 * 2 + 1) * sizeof(char));
  int di;

  md5_init(&HASH);
  md5_append(&HASH, (const md5_byte_t *) input, strlen(input));
  md5_finish(&HASH, digest);

  for (di = 0; di < 16; ++di) {
    sprintf(hex_output + di * 2, "%02x", digest[di]);
  }
  *output = hex_output;
}

unsigned int hashtab_hash(char *key) {
  unsigned int h = 0;
  char *p = key;
  int len = strlen(key);
  int hash, i;

  for (p = key; *p != '\0'; p++)
    h = h * HASHTAB_MUL + (unsigned int)(*p);

  for(hash = i = 0; i < len; ++i) {
      hash += p[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return (hash + h ) % HASHTAB_SIZE;
}
