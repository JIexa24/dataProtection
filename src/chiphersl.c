#include "../include/main.h"

long int vernam(char* in, char* out) {

}

long int vernam_decode(char* in) {
  int fdin, fdout;
  char c = '\0';
  char keych = '\0';
  char out[256] = {0};
  strcat(out, in);
  strcat(out, ".decode");
  char *keystr = malloc(sizeof(char));
  char cipherstr[8] = "vernam";
  long int k = 0;
  long int keyi = 0;
  long int ki = 0;
  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }
  if ((fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fdin);
    return -1;
  }

  if (read(fdin, cipherstr, 7 * sizeof(char)) == 0) return 0;
  while (read(fdin, &c, sizeof(char)) != 0) {
    keystr[ki] = c;
    ++ki;
    keystr = realloc(keystr, sizeof(char) * (ki + 1));
  }
  keyi =  ki / 2;

  for (k = 0; k < keyi; ++k) {
    c = keystr[k] ^ keystr[k + keyi];
    write(fdout, &c, sizeof(char));
  }
  closefiles(2, fdin, fdout);
  return k;
}

long int vernam_encode(char* in) {
  int fdin, fdout;
  char c = '\0';
  char key = 0;
  char *keystr = malloc(sizeof(char));
  char *keych = malloc(sizeof(char));
  char out[256] = {0};
  strcat(out, in);
  strcat(out, ".encode");
  long int k = 0;
  long int ki = 0;
  char cipherstr[8] = "vernam";
  srand(time(NULL));
  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }
  if ((fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fdin);
    return -1;
  }

  while (read(fdin, &c, sizeof(char)) != 0) {
    keych[ki] = (rand() % 256);//ascii table
    keystr[ki] = c ^ keych[ki];
    ++ki;
    keych = realloc(keych, sizeof(char) * (ki + 1));
    keystr = realloc(keystr, sizeof(char) * (ki + 1));
  }

  //it is encrypt
  write(fdout, &key, sizeof(char));
  write(fdout, cipherstr, 6 * sizeof(char));

  for (k = 0; k < ki; ++k) {
    write(fdout, &keych[k], sizeof(char));
  }
  for (k = 0; k < ki; ++k) {
    write(fdout, &keystr[k], sizeof(char));
  }

  closefiles(2, fdin, fdout);
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

long int generate_prime_too_number(long int e, long int min, long int max) {
    long int p = 1;
    do {
        p = (rand()) % (max - min) + min;
    } while (!test_prime_too_num(e, p));
    return p;
}

int rsa_generate() {
  unsigned long int p, q, e;
  long int eiler_res;
  unsigned long int d, n;
  int fdpub, fdpriv;

  if ((fdpub = open("./.keyrsa.pub",  O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file .keyrsa.pub\n");
    return -1;
  }
  if ((fdpriv = open("./.keyrsa",  O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
    printf("Can't open file .keyrsa\n");
    closefiles(1, fdpub);
    return -1;
  }
  long int nod = 0;
  while (nod != 1 || test_prime_num(d) == 0) {
    do {
      p = generate_prime_number(1, MAXINT);
      q = generate_prime_number(1, MAXINT);
    } while (p == q);
    n = p * q;
    eiler_res = (p - 1) * (q - 1);
    e = generate_prime_too_number(eiler_res, 1, eiler_res);

    equlid(e, eiler_res, &d, NULL, &nod);
    d = d % eiler_res;
  }
//  printf("rsa e = %lu, d = %lu,p = %lu, q = %lu, n = %lu\n", e, d, p, q, n);
  write(fdpub, &e, sizeof(e));
  write(fdpub, &n, sizeof(n));
  write(fdpriv, &d, sizeof(d));
  write(fdpriv, &n, sizeof(n));
  closefiles(2, fdpub, fdpriv);
  //pubkey - e, n
  //prkey - d,n
  return 0;
}

long int rsa_encode(char* in) {
  int fdin, fdout, fdkey;
  unsigned long int pubkey_e = 0, pubkey_n = 0;
  long int c = '\0';
  long int *keystr = malloc(sizeof(long int));
  long int k = 0;
  long int ki = 0;
  char key = 0;
  char cipherstr[5] = "rsa";
  char out[256] = {0};
  strcat(out, in);
  strcat(out, ".encode");
  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }
  if ((fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fdin);
    return -1;
  }
  if ((fdkey = open(".keyrsa.pub", O_RDONLY)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(2, fdin, fdout);
    return -1;
  }
  read(fdkey, &pubkey_e, sizeof(unsigned long int));
  read(fdkey, &pubkey_n, sizeof(unsigned long int));

  while (read(fdin, &c, sizeof(char)) != 0) {
//    printf("%ld ", c);
    c = mod_pow(c, pubkey_e, pubkey_n);
//    printf("%ld\n", c);
    keystr[ki] = c;
    ++ki;
    keystr = realloc(keystr, sizeof(long int) * (ki + 1));
    c = 0;
  }
  //it is encrypt
  write(fdout, &key, sizeof(char));
  write(fdout, cipherstr, 3 * sizeof(char));

  for (k = 0; k < ki; ++k) {
    write(fdout, &keystr[k], sizeof(long int));
  }

  closefiles(3, fdin, fdout, fdkey);
  return k;
}

long int rsa_decode(char* in) {
  int fdin, fdout, fdkey;
  unsigned long int privkey_d, privkey_n;
  long int  c = '\0';
  char *keystr = malloc(sizeof(char));
  long int k = 0;
  long int ki = 0;
  char cipherstr[5] = "rsa";
  char out[256] = {0};
  strcat(out, in);
  strcat(out, ".decode");
  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }
  if ((fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fdin);
    return -1;
  }

  if ((fdkey = open(".keyrsa", O_RDONLY)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(2, fdin, fdout);
    return -1;
  }
  read(fdkey, &privkey_d, sizeof(unsigned long int));
  read(fdkey, &privkey_n, sizeof(unsigned long int));

  if (read(fdin, cipherstr, 4 * sizeof(char)) == 0) return 0;
  while (read(fdin, &c, sizeof(long int)) != 0) {
    c = mod_pow(c, privkey_d, privkey_n);
    keystr[ki] = c;
    ++ki;
    keystr = realloc(keystr, sizeof(char) * (ki + 1));
    c = 0;
    ++k;
  }

  for (k = 0; k < ki; ++k) {
    write(fdout, &keystr[k], sizeof(char));
  }
  closefiles(3, fdin, fdout, fdkey);
  return k;
}


void shamir_generate() {

}

void shamir_encode() {

}

void shamir_decode() {

}

int el_gamal_generate() {
  long int y, x;
  long int g, p;
  int fdpub, fdpriv;

  if ((fdpub = open("./.keyeg.pub",  O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file .keyeg.pub\n");
    return -1;
  }
  if ((fdpriv = open("./.keyeg",  O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
    printf("Can't open file .keyeg\n");
    closefiles(1, fdpub);
    return -1;
  }
  p = generate_prime_number(0, MAXINT);
  x = generate_prime_number(1, p - 1);
  g = fsqrt_mod(p);
  //pubkey y, privkey x

}

int fsqrt_mod (long int p) {
  long int feiler = p - 1;
  unsigned long int var;
  long int i = 0, j = 0;
  char ok = 0;
  for (i = 1; i < feiler; ++i) {
    ok = 1;
    for (j = 1; j < feiler; ++j) {
      if ((var = mod_pow(i, j, p)) == 1) ok = 0;
    }
    if (ok == 1) break;
  }

  if (var = mod_pow(i, feiler, p) == 1) return i;
	return -1;
}

void el_gamal_encode() {

}

void el_gamal_decode() {

}

void closefiles(int N, ...) {
  int* p = &N;
  int i = 1;
  p++;
  for (i = 1; i <= N; ++i) {
    if (*p == -1) continue;
//      printf("close fd %d, N %d, i %d\n", *p, N, i);
    close(*p);
    p++;
  }
}
