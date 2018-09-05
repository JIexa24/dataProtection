#include "firstlab.h"

// num^pow % mod = result
long int fast_pow(long int num, unsigned long int pow,unsigned long int mod) {
    long int result = 1;
    while (pow)
    {
        if (pow & 0x1)
          result = (result * num) % mod;
        num = (num * num) % mod;
        pow = pow >> 1;
    }
    return result;
}
//ax+by = nod(a,b)
//http://www.intuit.ru/studies/courses/691/547/lecture/12389?page=2
void equlid(long int a, long int b, long int* x, long int *y, long int* nod) {
  long int Ux = a, Uy = 1, Uz= 0;
  long int Vx = b, Vy = 0, Vz = 1;
  long int Tempx, Tempy, Tempz;
  long int tmp;

  while (Vx != 0) {
    tmp = Ux / Vx;
    Tempx = Ux % Vx;
    Tempy = Uy - tmp * Vy;
    Tempz = Uz - tmp * Vz;
    Ux = Vx;
    Uy = Vy;
    Uz = Vz;
    
    Vx = Tempx;
    Vy = Tempy;
    Vz = Tempz;
  }
  
  *x = Uy;
  *y = Uz;
  *nod = Ux;
}
//ru.wikipedia.org/wiki/Протокол_Диффи_-_Хеллмана
void key_diffyhellman(long int* K1, long int* K2) {
  srand(time(NULL));
  //A know g p
  long int g = rand(), p = rand();
  long int Akey = 0, Bkey = 0;
  long int a, b;
  long int Ka, Kb;
  //A gen a & key
  a = rand();  
  Akey = fast_pow(g, a, p);
  //B gen b &key
  b = rand();  
  Bkey = fast_pow(g, b, p);
  
  Ka = fast_pow(Bkey, a, p);
  Kb = fast_pow(Akey, b, p);
  //Ba mod p = (gb mod p)a mod p = gab mod p = (ga mod p)b mod p = Ab mod p
  *K1 = Ka;
  *K2 = Kb;
}

int test_prime_num(long int p) {
  int i = 1;
  
  for (i = 1; i < p; ++i) {
    if (p % i == 0) return 0;
  }
  return 1;
}

long int generate_prime_number() {
    long int p = 1;
    do {
        p = (rand() + 1) % (500);
    } while (!test_prime_num(p));
    return p;
}

//g^x (mod p) = a 
//ru.wikipedia.org/wiki/Алгоритм_Гельфонда_-_Шенкса
long int small_big_steps(long int g, long int a, long int p) {
    long int res, i = 0, j = 0;
    long int m = (long int)sqrt((double)p) + 1;
    long int k = m;
    if (k * m <= p) {
      printf("k * m <= p\n");
      return -1;
    }
    for (i = 0; i < m; ++i) {
      for (j = 0; j <= k; ++j) {
        if (a * fast_pow(g, i, p) == fast_pow(g, j * m, p)) {
          return j * m - i;
        }
      }
    }
    return -1;
}
