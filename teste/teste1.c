#include <stdlib.h>
#include <stdio.h>
#include "../include/t2fs.h"

int main(){
  char nomes[100];
  if(identify2(nomes, (sizeof(char)*100)) == 0){
    printf("\n%s\n", nomes);
  } else {
    printf("ERRO");
 }
}
