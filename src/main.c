#include "../include/firstlab.h"
#include "../include/secondlab.h"
int main (int argc, char** argv) {
  long int x, y, nod; 
  long int Ka, Kb;

  equlid(11, 7, &x, &y,&nod);
  key_diffyhellman(&Ka, &Kb);

  printf("%ld\n", fast_pow(175,235,257)); 
  printf("%ld %ld %ld\n", x, y, nod);
  if (Ka == Kb) {
    printf("%ld=%ld\n", Ka, Kb);
  }
  
  return 0;
}
