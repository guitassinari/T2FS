#include "../include/inode.h"
#include "../include/t2fs.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
  struct t2fs_inode * inodeBuffer = malloc(sizeof(struct t2fs_inode));
  inodeBuffer->dataPtr[0] = 1;
  inodeBuffer->dataPtr[1] = 2;
  inodeBuffer->singleIndPtr = 3;
  inodeBuffer->doubleIndPtr = 4;
  int inodeIndex = writeInode(inodeBuffer);
  inodeBuffer = malloc(sizeof(struct t2fs_inode));
  readInode(inodeIndex, inodeBuffer);
  printf("%d - %d - %d - %d\n", inodeBuffer->dataPtr[0], inodeBuffer->dataPtr[1], inodeBuffer->singleIndPtr, inodeBuffer->doubleIndPtr);
}
