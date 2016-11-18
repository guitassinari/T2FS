#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/superblock.h"
#include "../include/t2fs.h"

#define SUPERBLOCK_ADDRESS 0
#define ERROR -1
#define SUCCESS 0

int identify2 (char *name, int size){
  char * identification = "Guilherme 00231060 | Caroline 00229753";
  int identificationSize = strlen(identification)*sizeof(char);
  if(size < identificationSize) return ERROR;
  strcpy(name, identification);
  return SUCCESS;
}

int mkdir2 (char *pathname){
  readSuperBlock();
}
