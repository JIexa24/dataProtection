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
  while (nod != 1 || test_prime_num(d) == 0 || d > 0xFFFF) {
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

int shamir_cipher(char* input_file)
{
  int is_it_ok = 0;
  char symb = '\0';
  long int ki = 0;
  char out[256] = {0};
  strcat(out, input_file);
  strcat(out, ".encode");
  unsigned long int p, c[2], d[2], x[2];
  int fd_input, fd_encoder, fd_decoder;
  unsigned long int* keystr_input = malloc(sizeof(unsigned long int));
  char* keystr_output = malloc(sizeof(char));

  if ((fd_input = open(input_file, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", input_file);
    return -1;
  }

  shamir_generate(&p, c, d);

  printf("p %lu c %lu %lu d %lu %lu\n", p, c[0],c[1],d[0],d[1]);
  while (read(fd_input, &symb, sizeof(char)) != 0) {
  printf("r - %c\n", symb);
    x[0] = mod_pow(symb, c[0], p);
    x[1] = mod_pow(x[0], c[1], p);
    x[0] = mod_pow(x[1], d[0], p);
    x[1] = mod_pow(x[0], d[1], p);
  printf("w - %lu\n", x[1]);
	  
    keystr_input[ki] = x[0];
    keystr_output[ki] = (char)x[1];
    if (symb != keystr_output[ki]) {
      is_it_ok = -1;
    }
  printf("i - %lu\n", is_it_ok);
    ++ki;
    keystr_input = realloc(keystr_input, sizeof(unsigned long int) * (ki + 1));
    keystr_output = realloc(keystr_output, sizeof(char) * (ki + 1));
    symb = 0;
  }

  if ((fd_encoder = open(out,  O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    return -1;
  }
  strcat(out, ".decode");
  if ((fd_decoder = open(out,  O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fd_encoder);
    return -1;
  }


  for (int k = 0; k < ki; ++k) {
    write(fd_encoder, &keystr_input[k], sizeof(unsigned long long int));
    write(fd_decoder, &keystr_output[k], sizeof(char));
  }

  closefiles(3, fd_input, fd_encoder, fd_decoder);

  if (is_it_ok != 0) return -1;
  return 0;
}

void shamir_generate(unsigned long int* p, unsigned long int* c, unsigned long int* d)
{
  unsigned long int euclid_res[3];
  *p = generate_prime_number(1, MAXINT);
  do {
  c[0] = generate_prime_too_number(*p - 1, 1, *p - 1);
  c[1] = generate_prime_too_number(*p - 1, 1, *p - 1);
  equlid(*p - 1, c[0],&d[0],NULL,NULL);
  equlid(*p - 1, c[1],&d[1],NULL,NULL);
  } while (d[0] > 0xFFFFFF || d[1] > 0xFFFFFF);
}


int elgamal_generate()
{
  unsigned long int p, g, x, y;
  int fd_public, fd_private;

  if ((fd_public = open("./.keyelgamal.pub",  O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file .keyelgamal.pub\n");
    return -1;
  }
  if ((fd_private = open("./.keyelgamal",  O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
    printf("Can't open file .keyelgamal\n");
    closefiles(1, fd_public);
    return -1;
  }

  p = generate_prime_number(255, 800);
//  p = 257;
  x = generate_prime_number(1, p - 1);
//  x = 3;
  g = fsqrt_mod(p);
//  g = 3; 
  y = mod_pow(g, x, p);

  write(fd_public, &y, sizeof(unsigned long int));
  write(fd_public, &p, sizeof(unsigned long int));
  write(fd_public, &g, sizeof(unsigned long int));
  write(fd_private, &x, sizeof(unsigned long int));
  write(fd_private, &p, sizeof(unsigned long int));
  write(fd_private, &g, sizeof(unsigned long int));

  closefiles(2, fd_public, fd_private);
  return 0;
}

long int elgamal_encode(char* input_file)
{
  int fd_input, fd_output, fd_key;
  char c = '\0';
  char key = 0;
  unsigned long int *keystr_a = malloc(sizeof(unsigned long int));
  unsigned long int *keystr_b = malloc(sizeof(unsigned long int));
  char out[256] = {0};
  strcat(out, input_file);
  strcat(out, ".encode");
  long int k = 0;
  long int ki = 0;
  unsigned long long int p, g, a = 1, b = 1;
  unsigned long long int pubkey = 0, sessionkey = 0;
  char cipherstr[9] = "elgamal";
  srand(time(NULL));
  if ((fd_input =  open(input_file, O_RDONLY)) == -1) {
    printf("[ERROR] Can't open file %s\n", input_file);
    return -1;
  }
  if ((fd_output = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fd_input);
    return -1;
  }
  if ((fd_key = open(".keyelgamal.pub", O_RDONLY)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(2, fd_input, fd_output);
    return -1;
  }
  read(fd_key, &pubkey, sizeof(unsigned long int));
  read(fd_key, &p, sizeof(unsigned long int));
  read(fd_key, &g, sizeof(unsigned long int));
	
  sessionkey = generate_prime_number(1, p - 1);
  while (read(fd_input, &c, sizeof(char)) != 0) { 
    a = mod_pow(g, sessionkey, p);
    b = mod_pow(pubkey, sessionkey, p);
    b = (b % p) * (c % p);
    keystr_a[ki] = a;
    keystr_b[ki] = b;
    ++ki;
    keystr_a = realloc(keystr_a, sizeof(unsigned long int) * (ki + 1));
    keystr_b = realloc(keystr_b, sizeof(unsigned long int) * (ki + 1));
    a = 0;
    b = 0;
//    printf("%d\n", c);
    c = 0;
  }

  write(fd_output, &key, sizeof(char));
  write(fd_output, cipherstr, 7 * sizeof(char));

  for (k = 0; k < ki; ++k) {
    write(fd_output, &keystr_a[k], sizeof(unsigned long int));
    write(fd_output, &keystr_b[k], sizeof(unsigned long int));
 //   printf("WRITE a = %ld, b = %ld\n", keystr_a[k], keystr_b[k]);
  }
  closefiles(3, fd_input, fd_output, fd_key);

  return k;
}

long int elgamal_decode(char* input_file)
{
  int fd_input, fd_output, fd_key;
  unsigned long long int privkey_x, privkey_p, privkey_g;
  unsigned long long int stream_a = '\0';
  unsigned long long int stream_b = '\0';
  unsigned long long int encode_message = 0;
  char *keystr = malloc(sizeof(char));
  long int k = 0;
  long int ki = 0;
  char cipherstr[7] = "elgamal";
  char out[256] = {0};
  strcat(out, input_file);
  strcat(out, ".decode");
  if ((fd_input =  open(input_file, O_RDONLY)) == -1) {
    printf("Can't open file %s\n", input_file);
    return -1;
  }
  if ((fd_output = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(1, fd_input);
    return -1;
  }

  if ((fd_key = open(".keyelgamal", O_RDONLY)) == -1) {
    printf("Can't open file %s\n", out);
    closefiles(2, fd_input, fd_output);
    return -1;
  }
  read(fd_key, &privkey_x, sizeof(unsigned long int));
  read(fd_key, &privkey_p, sizeof(unsigned long int));
  read(fd_key, &privkey_g, sizeof(unsigned long int));

  if (read(fd_input, cipherstr, 8 * sizeof(char)) == 0) return 0;
  while (read(fd_input, &stream_a, sizeof(unsigned long int)) != 0) {
    read(fd_input, &stream_b, sizeof(unsigned long int));
 //   printf("READ a = %ld, b = %ld\n",stream_a, stream_b);
    encode_message = mod_pow(stream_a, privkey_p - 1 - privkey_x, privkey_p);
    encode_message = ((encode_message % privkey_p) * (stream_b % privkey_p)) % privkey_p;
 //   printf("%ld (mod = %ld, x = %ld)\n", encode_message, privkey_p, privkey_x);
    keystr[ki] = encode_message;
    ++ki;
    keystr = realloc(keystr, sizeof(char) * (ki + 1));

    ++k;
  }

  for (k = 0; k < ki; ++k) {
    write(fd_output, &keystr[k], sizeof(char));
  }
  closefiles(3, fd_input, fd_output, fd_key);

  return k;
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
