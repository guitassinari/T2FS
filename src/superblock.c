#include <stdio.h>
#include <string.h>
#include "../include/superblock.h"
#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include "../include/bool.h"

static struct t2fs_superbloco superBlock;
static bool alreadyGotSuperBlock = false;

int readSuperBlock(){
  if(alreadyGotSuperBlock) return SUCCESS;
  unsigned char buffer[SECTOR_SIZE];
  if(read_sector(SUPERBLOCK_SECTOR, buffer) != SUCCESS) return ERROR;
  memcpy(&superBlock, &buffer, sizeof(struct t2fs_superbloco));
  alreadyGotSuperBlock = true;
}

int getId(char * buffer){
  if(readSuperBlock() == ERROR) return ERROR;
  memcpy(buffer, &(superBlock.id), sizeof(superBlock.id));
  return SUCCESS;
}

int getVersion(WORD * buffer){
  if(readSuperBlock() == ERROR) return ERROR;
  memcpy(buffer, &(superBlock.version), sizeof(WORD));
  return SUCCESS;
}

int getSuperBlockSize(WORD * buffer){
  if(readSuperBlock() == ERROR) return ERROR;
  memcpy(buffer, &superBlock.superblockSize, sizeof(superBlock.superblockSize));
  return SUCCESS;
}


int getFreeBlocksBitmapSize(WORD * buffer){
  if(readSuperBlock() == ERROR) return ERROR;
  memcpy(buffer, &superBlock.freeBlocksBitmapSize, sizeof(superBlock.freeBlocksBitmapSize));
  return SUCCESS;
}

int getFreeInodeBitmapSize(WORD * buffer){
  if(readSuperBlock() == ERROR) return ERROR;
  memcpy(buffer, &superBlock.freeInodeBitmapSize, sizeof(superBlock.freeInodeBitmapSize));
  return SUCCESS;
}

int getInodeAreaSize(WORD * buffer){
  if(readSuperBlock() == ERROR) return ERROR;
  memcpy(buffer, &superBlock.inodeAreaSize, sizeof(superBlock.inodeAreaSize));
  return SUCCESS;
}

int getBlockSize(WORD * buffer){
  if(readSuperBlock() == ERROR) return ERROR;
  memcpy(buffer, &superBlock.blockSize, sizeof(superBlock.blockSize));
  return SUCCESS;
}

int getDiskSize(DWORD * buffer){
  if(readSuperBlock() == ERROR) return ERROR;
  memcpy(buffer, &superBlock.diskSize, sizeof(superBlock.diskSize));
  return SUCCESS;
}
