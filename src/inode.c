#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/inode.h"
#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include "../include/bool.h"
#include "../include/bitmap2.h"

static int INODE_AREA_SECTOR = 0;
static int INODE_AREA_SIZE = 0;

#define EMPTY_INODE 0
#define INODE_IN_USE 1

/*
  Retorna o endereço do setor onde se inicia a area de Inodes
*/
int getInodeAreaSector(){
  if(INODE_AREA_SECTOR != 0) return INODE_AREA_SECTOR;
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
  int inodeIndex = searchBitmap2(BITMAP_INODE, EMPTY_INODE);
  if(inodeIndex <= 0) return ERROR;
  return inodeIndex;
}

/*
  Seta o status e um índice de inode como vazio
*/
int setInodeAsEmpty(){
  return setInodeStatus(inodeIndex, EMPTY_INODE);
}

/*
  Seta o status e um índice de inode como em uso
*/
int setInodeAsInUse(int inodeIndex){
  return setInodeStatus(inodeIndex, INODE_IN_USE);
}

/*
  Seta o status e um índice de inode (vazio ou em uso)
*/
int setInodeStatus(int inodeIndex, int status){
  if(setBitmap2(BITMAP_INODE, inodeIndex, status) == 0) return ERROR;
  return SUCCESS;
}

/*
  retorna 'true' se o indice passado estiver vazio
*/
int inodeIsEmpty(int inodeIndex){
  if(checkInodeStatus(inodeIndex) == EMPTY_INODE) return true;
  return false;
}

/*
  retorna o status de um indice de inode
*/
int checkInodeStatus(int inodeIndex){
  return getBitmap2(BITMAP_INODE, blockIndex);
}

/*
  retorna o tamanho da area reservadas para inodes no disco
*/
int inodeAreaSize(){
  if(INODE_AREA_SIZE != 0) return INODE_AREA_SIZE;
  WORD * inodeAreaSize = malloc(sizeof(WORD));
  getInodeAreaSize(inodeAreaSize);
  INODE_AREA_SIZE = ((*inodeAreaSize))/SECTOR_SIZE;
  free(inodeAreaSize);
  return INODE_AREA_SIZE;
}

/*
  Lê um inode e o coloca no parametro buffer
*/
int readInode(int inodeIndex, unsigned char * buffer){
  int secAddress = parseBlockNumberToSectorNumber(blockNumber);
  unsigned char * sectorBuffer = malloc(SECTOR_SIZE);
  int i;
  for(i = 0; i < getBlockSectorsNumber(); i++){
    if(read_sector(secAddress+i, sectorBuffer) == ERROR) return ERROR;
    memcpy(&(buffer[SECTOR_SIZE*i]), sectorBuffer, SECTOR_SIZE);
  }
  free(sectorBuffer);
  return SUCCESS;

  //TODO : Checar no bitmap se está valido
  int inodeSector = calculateInodeSector(inodeIndex);
  unsigned char * inodeBuffer = malloc(sizeof(struct t2fs_inode));
  unsigned char * sectorBuffer = malloc(SECTOR_SIZE);
  if(read_sector(inodeSector, sectorBuffer) == ERROR) return ERROR;
  //TODO : encontrar inode dentro do setor. Varrer um por um com memcpy(sizeof(inode)) ??
  return SUCCESS;
}

/*
  Escreve um inode em disco no primeiro espaço vazio encontrado
*/
int writeInode(int inodeNumber){
  //TODO : Checar no bitmap se está valido
}
