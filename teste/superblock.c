#include "../include/superblock.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  printf("\n");
  testId();
  testVersion();
  testSuperBlockSize();
  testFreeBlocksBitmapSize();
  testFreeInodeBitmapSize();
  testInodeAreaSize();
  testBlockSize();
  testDiskSize();
  printf("\n");
}

int testId(){
  char * buffer = malloc(sizeof(char)*4);
  getId(buffer);
  printf("ID : %c%c%c%c\n", buffer[0], buffer[1], buffer[2], buffer[3]);
}

int testVersion(){
  WORD * buffer = malloc(sizeof(WORD));
  getVersion(buffer);
  printf("Version : %d\n", (short)(*buffer));
}

int testSuperBlockSize(){
  WORD * buffer = malloc(sizeof(WORD));
  getSuperBlockSize(buffer);
  printf("Super Block Size : %d\n", (short)(*buffer));
}

int testFreeBlocksBitmapSize(){
  WORD * buffer = malloc(sizeof(WORD));
  getFreeBlocksBitmapSize(buffer);
  printf("Free Blocks Bitmap Size : %d\n", (short)(*buffer));
}

int testFreeInodeBitmapSize(){
  WORD * buffer = malloc(sizeof(WORD));
  getFreeInodeBitmapSize(buffer);
  printf("Free Inodes Bitmap Size : %d\n", (short)(*buffer));
}

int testInodeAreaSize(){
  WORD * buffer = malloc(sizeof(WORD));
  getInodeAreaSize(buffer);
  printf("Inode Area Size : %d\n", (short)(*buffer));
}

int testBlockSize(){
  WORD * buffer = malloc(sizeof(WORD));
  getBlockSize(buffer);
  printf("Block Size : %d\n", (short)(*buffer));
}

int testDiskSize(){
  DWORD * buffer = malloc(sizeof(DWORD));
  getDiskSize(buffer);
  printf("Disk Size : %d\n", (int)(*buffer));
}
