#include "../include/block.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  unsigned char * blockBuffer = malloc(blockSize());
  char * identification = "Guilherme 00231060 | Caroline 00229753";
  memcpy(blockBuffer, identification, strlen(identification)*sizeof(char));
  int blockPosition = writeBlock(blockBuffer);
  blockBuffer = malloc(blockSize());
  readBlock(blockPosition, blockBuffer);
  int i;
  for(i = 0; i <= strlen(identification); i++){
    printf("%c", blockBuffer[i]);
  }
}
