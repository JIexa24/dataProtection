#include "../include/main.h"

long int vernam_decode(char* in, char* out, char* key) {
  int fdin, fdout, fdkey;
  char c = '\0';
  char keych = '\0';
  long int k = 0;
  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }
  if ((fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fdin);
    return -1;
  }
  if ((fdkey = open(key, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", key);
    closefiles(2, fdin, fdout);
    return -1;
  }
  while (read(fdin, &c, 1) != 0) {
    if (read(fdkey, &keych, 1) == 0) {
      printf("\nlength files is not equal\n");
      return k;
    }
    c = c ^ keych;
    write(fdout, &c, 1);
    ++k;
  }
  closefiles(3, fdin, fdout, fdkey);
  return k;
}

long int vernam_encode(char* in, char* out, char* key) {
  int fdin, fdout, fdkey;
  char c = '\0';
  char keych = '\0';
  long int k = 0;
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
  if ((fdkey = open(key, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", key);
    closefiles(2, fdin, fdout);
    return -1;
  }
  while (read(fdin, &c, 1) != 0) {
    keych = (rand() % 256);//ascii table
    c = c ^ keych;
    write(fdout, &c, 1);
    write(fdkey, &keych, 1);
    ++k;
  }
  closefiles(3, fdin, fdout, fdkey);
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
  while (nod != 1) {
    do {
      p = generate_prime_number(1, MAXINT);
      q = generate_prime_number(1, MAXINT);
    } while (p == q);
    n = p * q;
    eiler_res = (p - 1) * (q - 1);
    e = generate_prime_too_number(eiler_res, 1, eiler_res);
/*  while (d <= 0xFFFFFFFF) {
      if ((d * e) % eiler_res == 1) break;
      ++d;
    }
*/
    equlid(e, eiler_res, &d, NULL, &nod);
  }
  printf("rsa e = %lu, d = %lu %X,p = %lu, q = %lu, n = %lu\n", e, d,d, p, q, n);
  write(fdpub, &e, sizeof(e));
  write(fdpub, &n, sizeof(n));
  write(fdpriv, &d, sizeof(d));
  write(fdpriv, &n, sizeof(n));
  closefiles(2, fdpub, fdpriv);
  //pubkey - e, n
  //prkey - d,n
  return 0;
}

long int rsa_encode(char* in, char* out) {
  int fdin, fdout, fdkey;
  unsigned long int pubkey_e, pubkey_n;
  long int k = 0;
  long int c = '\0';
  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }
  if ((fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fdin);
    return -1;
  }
  if ((fdkey = open("./.keyrsa.pub", O_RDONLY)) == -1) {
    printf("Can't open file %s\n", "./.keyrsa.pub");
    closefiles(2, fdin, fdout);
    return -1;
  }

  if (read(fdkey, &pubkey_e,  sizeof(pubkey_e)) == 0) {
    printf("Key e loss %s\n", "./.keyrsa.pub");
    closefiles(3, fdin, fdout, fdkey);
    return -1;
  }

  if (read(fdkey, &pubkey_n, sizeof(pubkey_n)) == 0) {
    printf("Key n loss %s\n", "./.keyrsa.pub");
    closefiles(3, fdin, fdout, fdkey);
    return -1;
  }

  while (read(fdin, &c, 1) != 0) {
//    printf("%ld ", c);
    c = mod_pow(c, pubkey_e, pubkey_n);
//    printf("%ld\n", c);
    write(fdout, &c, sizeof(c));
    c = 0;
    ++k;
  }
  closefiles(3, fdin, fdout, fdkey);
  return k;
}

long int rsa_decode(char* in, char* out) {
  int fdin, fdout, fdkey;
  unsigned long int privkey_d, privkey_n;
  long int k = 0;
  long int  c = '\0';
  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }
  if ((fdout = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fdin);
    return -1;
  }
  if ((fdkey = open("./.keyrsa", O_RDONLY)) == -1) {
    printf("Can't open file %s\n", "./.keyrsa");
    closefiles(2, fdin, fdout);
    return -1;
  }

  if (read(fdkey, &privkey_d, sizeof(privkey_d)) == 0) {
    printf("Key d loss %s\n", ".keyrsa");
    closefiles(3, fdin, fdout, fdkey);
    return -1;
  }

  if (read(fdkey, &privkey_n, sizeof(privkey_n)) == 0) {
    printf("Key n loss %s\n", ".keyrsa");
    closefiles(3, fdin, fdout, fdkey);
    return -1;
  }

  while (read(fdin, &c, sizeof(c)) != 0) {
//    printf("%ld ", c);
    c = mod_pow(c, privkey_d, privkey_n);
//    printf("%ld\n", c);
    write(fdout, &c, 1);
    c = 0;
    ++k;
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
