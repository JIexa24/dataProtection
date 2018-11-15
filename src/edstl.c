
#include "../include/main.h"

int eds_rsa(char* in) {
  int fdin, fdout;
  int fdpub, fdpriv;
  int i = 0;
  unsigned long long int ki = 0, hash_i;
  unsigned long long int n = 0, d = 0, e = 0;
  char c = '\0';
  char keych = '\0';
  char out[256] = {0};
  char *keystr = malloc(sizeof(char));
  unsigned long long int *keystrs = malloc(sizeof(unsigned long long int) * 32);
  char *hashkeystr = NULL;

  if ((fdin = open(in, O_RDONLY)) == -1) {
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
  if ((fdpriv = open(".keyrsa", O_RDONLY)) == -1) {
    printf("Can't open file %s\n", ".keyrsa");
    closefiles(2, fdin, fdpub);
    return -1;
  }
  strcat(out, in);
  strcat(out, ".rsgn");
  if ((fdout =  open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    return -1;
  }

  read(fdpriv, &d, sizeof(unsigned long long int));
  read(fdpriv, &n, sizeof(unsigned long long int));
  printf("result - %s\n", hashkeystr);
  for (i = 0; i < 32; ++i) {
    keystrs[i] = mod_pow(hashkeystr[i], d, n);
    write(fdout, &keystrs[i], sizeof(keystrs[i]));
  }
  free(hashkeystr);
  closefiles(3, fdin, fdout, fdpriv);
  return 0;
}

int eds_rsa_check(char* in, char* inm) {
  int fdin, fdinm, fdout;
  int fdpub, fdpriv;
  int i = 0;
  unsigned long long int ki = 0, hash_i;
  unsigned long long int n = 0, d = 0, e = 0;
  unsigned long long int c = '\0';
  char keych = '\0';
  char out[256] = {0};
  char *keystr = malloc(sizeof(char));
  char *hashkeystre;
  char hashkeystr[32];

  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    closefiles(1, fdin);
    return -1;
  }

  if ((fdinm = open(inm, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    closefiles(1, fdin);
    return -1;
  }

  while (read(fdinm, &c, sizeof(char)) != 0) {
    keystr[ki] = c;
    ++ki;
    keystr = realloc(keystr, sizeof(char) * (ki + 1));
  }
  keystr[ki] = 0;
  hash(keystr, &hashkeystre);

  if ((fdpub = open(".keyrsa.pub", O_RDONLY)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fdin);
    return -1;
  }
  read(fdpub, &e, sizeof(unsigned long long int));
  read(fdpub, &n, sizeof(unsigned long long int));

  for (i = 0; i < 32; ++i) {
    read(fdin, &c, sizeof(unsigned long long int));
    hashkeystr[i] = mod_pow(c, e, n);
  }
  printf("expect - %s\n", hashkeystre);
  printf("real   - %s\n", hashkeystr);
  closefiles(3, fdin, fdpub, fdout);
  return 0;
}

int eds_elgamal(char* in) {
  int fdin, fdout;
  unsigned long long int ki = 0, hash_i;
  char c = '\0';
  char keych = '\0';
  char out[256] = {0};
  char *keystr = malloc(sizeof(char));
  char *hashkeystr = NULL;
  unsigned long long int p = 0;
  unsigned long long int q = 0;
  unsigned long long int k;
  while (!q) {
      p = generate_prime_number(350, MAXINT);
      if (test_prime_num((p - 1) / 2)) {
      q = (p - 1) / 2;
    }
  }
  k = generate_prime_too_number(p- 1, 2, p-2);
  unsigned long long int k1 = 1, nod = 0;
  unsigned long long int g = fsqrt_mod(p);
  unsigned long long int r = mod_pow(g, k, p);
  unsigned long long int x = generate_prime_too_number(p-1, 300, p - 2 );
  unsigned long long int y = mod_pow(g, x, p);
  long long int s = 0;
  unsigned long long int testl = 0;
  unsigned long long int testr = 0;
  do {
    nod = (++k1 * k) % (p - 1);
    if (k1 == p - 1) {
      k = generate_prime_too_number(p-1,1,p-1);
      k1 = 1;
      nod = 0;
    }
    if ( nod == 1) break;
//    printf("k1 = %llu", k1);
  } while (1);
  printf("p = %llu, g = %llu, x = %llu, y = %llu, k = %llu, r = %llu, s = %llu, k1 = %llu\n\n", p, g, x, y, k, r, s, k1);

  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    return -1;
  }
  strcat(out, in);
  strcat(out, ".esgn");
  if ((fdout =  open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    return -1;
  }

  while (read(fdin, &c, sizeof(char)) != 0) {
    keystr[ki] = c;
    ++ki;
    keystr = realloc(keystr, sizeof(char) * (ki + 1));
  }

  keystr[ki] = 0;

  hash(keystr, &hashkeystr);
  int i = 0;
  write(fdout, &y, sizeof(y));
  write(fdout, &g, sizeof(g));
  write(fdout, &p, sizeof(p));
  for(i = 0; i < 32;++i) {
    s = 0;
    //s = s + ((hashkeystr[i] * k1) % (p - 1) - (x * r * k1) % (p - 1)) % (p - 1);
    s = ((hashkeystr[i] - x * r)) % (p - 1);
    s += s < 0 ? p - 1 : 0;
    s = (k1 * s) % (p - 1);
    write(fdout, &r, sizeof(r));
    write(fdout, &s, sizeof(s));
  }
//  printf("%s\n%s ---- %llu\neds = %llu, %llu\n\n", keystr, hashkeystr, hash_i, r, s);
//
}

int eds_elgamal_check(char* in, char* inm) {
  int fdin, fdinm, fdout;
  int fdpub, fdpriv;
  int i = 0;
  unsigned long long int ki = 0, hash_i;
  unsigned long long int n = 0, r = 0, p = 0, y = 0, g = 0, ps = 0;
  long long int s=0;
  unsigned long long int c = '\0', tr = 0, tl = 0;
  char keych = '\0';
  char out[256] = {0};
  char *keystr = malloc(sizeof(char));
  char *hashkeystre;
  char hashkeystr[32];

  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    closefiles(1, fdin);
    return -1;
  }

  if ((fdinm = open(inm, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    closefiles(1, fdin);
    return -1;
  }
  read(fdin, &y, sizeof(y));
  read(fdin, &g, sizeof(g));
  read(fdin, &p, sizeof(p));

  while (read(fdinm, &c, sizeof(char)) != 0) {
    keystr[ki] = c;
    ++ki;
    keystr = realloc(keystr, sizeof(char) * (ki + 1));
  }
  keystr[ki] = 0;
  hash(keystr, &hashkeystre);
  for (i = 0; i < 32; ++i) {
    read(fdin, &r, sizeof(r));
    read(fdin, &s, sizeof(s));
    tl = 0;
    tl = mod_pow(y, r, p);
    tl = tl * mod_pow(r, s, p);
    tl = tl % p;
    tr = mod_pow(g, hashkeystre[i], p);
    if ((0 < r && r < p) && (0 < s && s < (p - 1)) && (tl == tr)) {
      printf("correct \n");
    } else {
      printf("incorrect %llu == %llu %d %c s = %llu r = %llu g=%llu p=%llu\n", tl, tr, hashkeystre[i], hashkeystre[i], s,r,g,p);
    }
  }
  return 0;
}

int eds_gost(char* in) {
  int fdin, fdout;
  int fdpub, fdpriv;
  int i = 0;
  unsigned long long int ki = 0, hash_i;
  unsigned long long int n = 0, d = 0, e = 0,a,b,x,y,q,p,h,k;
  long long int r,s;
  char c = '\0';
  char keych = '\0';
  char out[256] = {0};
  char *keystr = malloc(sizeof(char));
  unsigned long long int *keystrs = malloc(sizeof(unsigned long long int) * 32);
  char *hashkeystr = NULL;
  strcat(out, in);
  strcat(out,".gsgn");
  if ((fdout =  open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    return -1;
  }
  do {
    q = generate_prime_number(300,MAXINT);
    b = random() % 1000;
    p = b * q + 1;
  } while (!test_prime_num(p));

  do {
    a = rand() % p;
    a = mod_pow(a, b, p);
  } while (a == 1);
  x = rand() % q;
  y = mod_pow(a, x, p);

  write(fdout, &p, sizeof(p));
  write(fdout, &q, sizeof(q));
  write(fdout, &a, sizeof(a));
  write(fdout, &y, sizeof(y));

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

  for (int i = 0; i < 32; i++) {
    h = hashkeystr[i];
    while (1) {
      k = rand() % q;
      r = mod_pow(a, k, p) % q;
      if (r == 0) {
        continue;
      }
      s = (k * h + x * r) % q;
      if (s > 0) {
        break;
      }
    }
    write(fdout, &r, sizeof(r));
    write(fdout, &s, sizeof(s));
  }
  return 0;
}
int eds_gost_check(char* in, char* inm) {
  int fdin, fdinm;
  int fdpub, fdpriv;
  int i = 0;
  unsigned long long int ki = 0, hash_i;
  unsigned long long int n = 0, d = 0, e = 0,a,b,x,y,q,p;
  long long int r, h, h_inv, s, gcd_x, u1, u2, v;
  char c = '\0';
  char keych = '\0';
  char out[256] = {0};
  char *keystr = malloc(sizeof(char));
  unsigned long long int *keystrs = malloc(sizeof(unsigned long long int) * 32);
  char *hashkeystr = NULL;

  if ((fdin =  open(in, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    closefiles(1, fdin);
    return -1;
  }

  if ((fdinm = open(inm, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", in);
    closefiles(1, fdin);
    return -1;
  }
  read(fdin, &p, sizeof(p));
  read(fdin, &q, sizeof(q));
  read(fdin, &a, sizeof(a));
  read(fdin, &y, sizeof(y));
  while (read(fdinm, &c, sizeof(char)) != 0) {
    keystr[ki] = c;
    ++ki;
    keystr = realloc(keystr, sizeof(char) * (ki + 1));
  }
  keystr[ki] = 0;
  hash(keystr, &hashkeystr);

  int flg = 0;
  for (int i = 0; i < 32; i++) {
    h = hashkeystr[i];
    read(fdin, &r, sizeof(r));
    read(fdin, &s, sizeof(s));
    if (r >= q || s >= q) {
      flg = 1;
      break;
    }
    equlid(h, q, &gcd_x, NULL, NULL);
    h_inv = gcd_x < 0 ? q + gcd_x : gcd_x;
    u1 = (s * h_inv) % q;
    u2 = (-r * h_inv) % q;
    u2 += u2 < 0 ? q : 0;
    v = ((mod_pow(a, u1, p) * mod_pow(y, u2, p)) % p) % q;
    if (v != r) {
      flg = 1;
      break;
    }
  }

  if (flg)
    return 1;
  return 0;
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
