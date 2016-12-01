#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include "../include/bool.h"
#include "../include/bitmap2.h"

static int INODE_AREA_SECTOR = 0;
static int INODE_AREA_SIZE = 0;

#define ROOT_DIR_INODE 0

#define EMPTY_INODE 0
#define INODE_IN_USE 1

/*
  Lê um inode e o coloca no parametro buffer
*/
int readInode(int inodeIndex, struct t2fs_inode * buffer){
  if(inodeIsEmpty(inodeIndex)) return ERROR;
  int sectorAddress = parseInodeNumberToSectorNumber(inodeIndex);
  int offset = inodeIndex%sectorAddress;
  unsigned char * sectorBuffer = malloc(SECTOR_SIZE);
  int i;
  if(read_sector(sectorAddress, sectorBuffer) == ERROR) return ERROR;
  memcpy(buffer, &(sectorBuffer[offset*inodeSize()]), inodeSize());
  free(sectorBuffer);
  return SUCCESS;
}

/*
  Escreve um inode em disco no primeiro espaço vazio encontrado
*/
int writeInode(struct t2fs_inode * buffer){
  int inodeIndex = searchEmptyInode();
  if(inodeIndex == ROOT_DIR_INODE){
    setInodeAsInUse(ROOT_DIR_INODE);
    inodeIndex = searchEmptyInode();
  }
  if(inodeIndex == ERROR) return ERROR;
  writeInodeAtIndex(inodeIndex, buffer);
  return inodeIndex;
}

int writeRootInode(struct t2fs_inode * buffer){
  setInodeAsInUse(ROOT_DIR_INODE);
  return writeInodeAtIndex(ROOT_DIR_INODE, buffer);
}

int writeInodeAtIndex(int inodeIndex, struct t2fs_inode * buffer){
  int sectorAddress = parseInodeNumberToSectorNumber(inodeIndex);
  int offset = inodeIndex%sectorAddress;
  unsigned char * sectorBuffer = malloc(SECTOR_SIZE);
  int i;
  if(read_sector(sectorAddress, sectorBuffer) == ERROR) return ERROR;
  memcpy(&(sectorBuffer[offset*inodeSize()]), buffer, inodeSize());
  if(write_sector(sectorAddress, sectorBuffer) == ERROR) return ERROR;
  setInodeAsInUse(inodeIndex);
  free(sectorBuffer);
}
/*
  Deletes an inode from disk
*/
int deleteInode(int inodeIndex){
  setInodeAsEmpty(inodeIndex);
}

int inodeSize(){
  return sizeof(struct t2fs_inode);
}

/*
  retorna ponteiro pra um novo inode vazio
*/
struct t2fs_inode * newInode(){
  struct t2fs_inode * inode = malloc(inodeSize());
  inode->dataPtr[0] = INVALID_PTR;
  inode->dataPtr[1] = INVALID_PTR;
  inode->singleIndPtr = INVALID_PTR;
  inode->doubleIndPtr = INVALID_PTR;
  return inode;
}

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

/*
  retorna o promeiro indice de inode vazio encontrado
*/
int searchEmptyInode(){
  int inodeIndex = searchBitmap2(BITMAP_INODE, EMPTY_INODE);
  if(inodeIndex <= 0) return ERROR;
  return inodeIndex;
}

/*
  Seta o status e um índice de inode como vazio
*/
int setInodeAsEmpty(int inodeIndex){
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
  return getBitmap2(BITMAP_INODE, inodeIndex);
}
/*
  retorna o indice do setor onde se encontro o inode de indice passado
*/
int parseInodeNumberToSectorNumber(int inodeIndex){
  return getInodeAreaSector() + floor(inodeIndex/inodesPerSector());
}

/*
  retornar o numero de inodes existentes dentro de um setor
*/
int inodesPerSector(){
  return SECTOR_SIZE/inodeSize();
}
