#include "../include/main.h"

long int vernam_decode(char* in, char* out, char* key) {
  srand(time(NULL));
  int fdin =  open(in, O_RDONLY);
  int fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  int fdkey = open(key, O_RDONLY);
  if (fdin == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }
  if (fdout == -1) {
    printf("Can't open file %s\n", out);
    return -1;
  }
  if (fdkey == -1) {
    printf("Can't open file %s\n", key);
    return -1;
  }
  char c = '\0';
  char keych = '\0';
  long int k = 0;
  while (read(fdin, &c, 1) != 0) {
    if (read(fdkey, &keych, 1) == 0) {
      printf("\nlength files is not equal\n");
      return k;
    }
    c = c ^ keych;
    write(fdout, &c, 1);
    ++k;
  }
  close(fdin);
  close(fdout);
  close(fdkey);
  return k;
}

long int vernam_encode(char* in, char* out, char* key) {
  long int fdin =  open(in, O_RDONLY);
  int fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  int fdkey = open(key, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (fdin == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }
  if (fdout == -1) {
    printf("Can't open file %s\n", out);
    return -1;
  }
  if (fdkey == -1) {
    printf("Can't open file %s\n", key);
    return -1;
  }
  char c = '\0';
  char keych = '\0';
  long int k = 0;
  while (read(fdin, &c, 1) != 0) {
    keych = (rand() % 256);//ascii table
    c = c ^ keych;
    write(fdout, &c, 1);
    write(fdkey, &keych, 1);
    ++k;
  }
  close(fdin);
  close(fdout);
  close(fdkey);
  return k;
}

int test_prime_too_num(long int p, long int e) {
  int i = 1;
  unsigned long int x, y, nod;
  if (p == e) return 0;
  if (p % e == 0) return 0;
  equlid(p, e, &x, &y, &nod);
  if (nod != 1) return 0;
  return 1;
}

long int generate_prime_too_number(long int e) {
    long int p = 1;
    do {
        p = (rand() + 1) % (500);
    } while (!test_prime_too_num(e, p));
    return p;
}

void rsa_generate() {
  long int p, q, e;
  long int eiler_res;
  long int d, n;
  int fdpub, fdpriv;
  do {
    p = generate_prime_number();
    q = generate_prime_number();
  } while (p != q);

  n = p * q;

  eiler_res = (p - 1) * (q - 1);
  e = generate_prime_too_number(eiler_res);

  d = fast_pow(e, (unsigned long int)-1, eiler_res);
  fdpub = open("~/.keyrsa.pub",  O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (fdpub == -1) {
    printf("Can't open file .keyrsa.pub\n");
    return;
  }
  fdpriv = open("~/.keyrsa",  O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if (fdpriv == -1) {
    printf("Can't open file .keyrsa\n");
    return;
  }
  write(fdpub, &e, 8);
  write(fdpub, &n, 8);
  write(fdpriv, &d, 8);
  write(fdpriv, &n, 8);
  close(fdpub);
  close(fdpriv);
  //pubkey - e, n
  //prkey - d,n
}

void rsa_encode(char* in, char* out) {
  int fdin, fdout, fdkey;

}

void rsa_decode(char* in, char* out) {
  int fdin, fdout, fdkey;

}
