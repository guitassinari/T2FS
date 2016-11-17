#include <stdlib.h>
#include <stdio.h>
#include "../include/t2fs.h"

int main(){
  struct t2fs_superbloco superBlock;
  printf("%s", superBlock->id);
}
