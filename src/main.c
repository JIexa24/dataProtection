#include "../include/main.h"

int main (int argc, char** argv) {  srand(time(NULL));
  if (argc < 2) {
    fprintf(stderr, "ERROR : Not enough arguments!\n");
    exit(EXIT_FAILURE);
  }
  int account_state = atoi(argv[1]);
  printf("INIT : Starting balance:\t%d\n",account_state);
  int payment_amount;

  bank_startup();

  while (account_state > 0) {
    printf("BANK : Enter the payment amount:\n");
    scanf("%d", &payment_amount);
    if (payment_amount == 0) {
      printf("BANK : Recieved zero payment! Exiting now...\n");
      break;
    }
    printf("BANK : Starting transaction...\n");
    if(!initialization_transaction(&account_state, payment_amount)) {
      printf("SHOP : Success!\n");
    } else {
      printf("SHOP : Payment rejected!\n");
      break;
    }
    account_state == 0 ? printf("INFO : Out of money. Exiting now...\n") : printf("INFO :  Current account state:\t%d\n\n",account_state);
  }
  return 0;
}
