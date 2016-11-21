#include <stdlib.h>
#include <stdio.h>
#include "../include/inode.h"
#include "../include/superblock.h"
#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include "../include/bool.h"

static int INODE_AREA_SECTOR = NULL;
static int INODE_AREA_SIZE = NULL;

int getInodeAreaSector(){
  if(INODE_AREA_SECTOR != NULL) return INODE_AREA_SECTOR;
  WORD * superBlockSize = malloc(sizeof(WORD));
  WORD * inodeBitmapSize = malloc(sizeof(WORD));
  WORD * blocksBitmapSize = malloc(sizeof(WORD));
  getSuperBlockSize(superBlockSize);
  getFreeInodeBitmapSize(inodeBitmapSize);
  getFreeBlocksBitmapSize(blocksBitmapSize);
  INODE_AREA_SECTOR = *superBlockSize + *inodeBitmapSize + *blocksBitmapSize;
  free(superBlockSize);
  free(inodeBitmapSize);
  free(blocksBitmapSize);
  return INODE_AREA_SECTOR;
}

int calculateInodeSector(int inodeIndex){
  INODE_AREA_SECTOR + inodeSectorOffset(inodeIndex);
}

int inodeSectorOffset(int inodeIndex){
  int inodesPerSector = SECTOR_SIZE/sizeof(struct t2fs_inode);
  return Math.floor(inodeIndex/inodesPerSector);
}

int searchEmptyInode(){

}

int setInodeAsEmpty(){

}

int setInodeAsInUse(){

}

setInodeStatus(){
  
}

int getInodeAreaSize(){
  if(INODE_AREA_SIZE != NULL) return INODE_AREA_SIZE;
  WORD * inodeAreaSize = malloc(sizeof(WORD));
  getInodeAreaSize(inodeAreaSize);
  INODE_AREA_SIZE = ((*inodeAreaSize))/SECTOR_SIZE;
  free(inodeAreaSize);
  return INODE_AREA_SIZE;
}

int readInode(int inodeIndex){
  //TODO : Checar no bitmap se está valido
  int inodeSector = calculateInodeSector(inodeIndex);
  unsigned char * inodeBuffer = malloc(sizeof(struct t2fs_inode));
  unsigned char * sectorBuffer = malloc(SECTOR_SIZE);
  if(read_sector(inodeSector, sectorBuffer) == ERROR) return ERROR;
  //TODO : encontrar inode dentro do setor. Varrer um por um com memcpy(sizeof(inode)) ??
  return SUCCESS;
}

int writeInode(int inodeNumber){
  //TODO : Checar no bitmap se está valido
}
