#include "../include/main.h"

// num^pow % mod = result
long int mod_pow(long int num, unsigned long int pow,unsigned long int mod) {
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

  if (x != NULL) *x = Uy;
  if (y != NULL) *y = Uz;
  if (nod != NULL) *nod = Ux;
}
//ru.wikipedia.org/wiki/��������_�����_-_��������
void key_diffyhellman(long int* K1, long int* K2) {
  srand(time(NULL));
  //A know g p
  long int g = generate_prime_number(1, 1000);//4200;
  long int p;
  p = generate_prime_number(1, 1000);//3571;
  long int Akey = 0, Bkey = 0;
  long int a, b;
  long int Ka, Kb;
  //A gen a & key
  a = rand() % (1000000000 - 10000) + 10000;//187653288;
  Akey = mod_pow(g, a, p);
  //B gen b &key
  b = rand() % (1000000000 - 10000) + 10000;// 987701673;
  Bkey = mod_pow(g, b, p);

  Ka = mod_pow(Bkey, a, p);
  Kb = mod_pow(Akey, b, p);
  //Ba mod p = (gb mod p)a mod p = gab mod p = (ga mod p)b mod p = Ab mod p
  *K1 = Ka;
  *K2 = Kb;
}

int test_prime_num(long int p) {
  int i = 1;

  for (i = 2; i < sqrt(p); ++i) {
    if (p % i == 0) return 0;
  }
  return 1;
}

long int gcd (long int a, long int b) {
  if (b == 0)
    return a;
  return gcd(b, a % b);
}

int test_prime_f(long int p) {
  int i = 1;
  long int a = 1;
  if (p == 2)
    return 1;
  srand(time(NULL));
  for (i = 0; i < 100; ++i) {
    a = (rand() % (p - 2)) + 2;
    if (gcd(a, p) != 1) {
      return 0;
    }
    if (mod_pow(a, p - 1, p) != 1) return 0;
  }
  return 1;
}

long int generate_prime_number(long int min, long int max) {
    long int p = 1;
    do {
        p = (rand()) % (max - min ) + min;
    } while (!test_prime_num(p));
    return p;
}

struct pair {
  long int val;
  long int index;
} *littles;

int cmp(const void * x1, const void * x2)   // функция сравнения элементов массива
{
  struct pair x11 = *(struct pair*)x1;
  struct pair x21 = *(struct pair*)x2;
  return (  x11.val - x21.val );              // если результат вычитания равен 0, то числа равны, < 0: x1 < x2; > 0: x1 > x2
}

long int binary_search(struct pair *a, long int n, long int val) {
  long int index_cursor = n/2;
  long int iteration = 0;
  while (1) {
    if (a[index_cursor].val > val) {
      index_cursor = (index_cursor + n)/ 4 ;
    } else index_cursor = (index_cursor + n) / 2;
    if (a[index_cursor].val == val) return a[index_cursor].index;
    if(++iteration >= n/2) break;
  }
  return -1;
}

//g^x (mod p) = a
long int small_big_steps(long int g, long int a, long int p) {
    long int res, i = 0, j = 0;
    long int m = floor((long int)sqrt((double)p)) + 1;
    long int k = m;
    struct pair *littles;

    if (k * m <= p) {
      printf("k * m <= p\n");
      return -1;
    }

    littles = (struct pair*)malloc(sizeof(struct pair) * (m + 1));
    long int bigs;
    littles[0].val = INT32_MAX;
    littles[0].index = i;
    for (i = 1; i < m; ++i) {
      littles[i].index = i;
      littles[i].val = (a * (long int)pow(g, i )) % p;
      if (littles[i].val == 1) littles[i].val = INT32_MAX;
    }

    qsort(littles, m + 1, sizeof(struct pair), cmp);

    for(int j = 1; j < m; j++){
      bigs = mod_pow(g, j*m, p);
      if((i = binary_search(littles, m, bigs)) != -1){
        return i * m - j;
      }
    }

    return -1;
}
/*

*/
