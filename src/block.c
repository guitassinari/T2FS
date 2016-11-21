#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/superblock.h"
#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include "../include/bool.h"
#include "../include/bitmap2.h"

#define EMPTY_BLOCK 0
#define BLOCK_IN_USE 1

static int BLOCK_AREA_SECTOR = 0;
/*
  Lê um bloco do disco e coloca no parametro Buffer
*/
int readBlock(int blockNumber, unsigned char * buffer){
  if(!blockIsEmpty(blockNumber)) return ERROR;
  int secAddress = parseBlockNumberToSectorNumber(blockNumber);
  unsigned char * sectorBuffer = malloc(SECTOR_SIZE);
  int i;
  for(i = 0; i < getBlockSectorsNumber(); i++){
    if(read_sector(secAddress+i, sectorBuffer) == ERROR) return ERROR;
    memcpy(&(buffer[SECTOR_SIZE*i]), sectorBuffer, SECTOR_SIZE);
  }
  free(sectorBuffer);
  return SUCCESS;
}

/*
Escreve um bloco no disco

retorno : número/indíce do BLOCO dentro do disco (não é o número do setor! é o índice do bloco!)
*/
int writeBlock(unsigned char * blockBuffer){
  int blockAddress = searchEmptyBlock(BITMAP_DADOS, EMPTY_BLOCK);
  if(blockAddress == ERROR) return ERROR;
  int secAddress = parseBlockNumberToSectorNumber(blockAddress);
  unsigned char * sectorBuffer = malloc(SECTOR_SIZE);
  int i;
  for(i = 0; i < getBlockSectorsNumber(); i++){
    memcpy(sectorBuffer, &(blockBuffer[SECTOR_SIZE*i]), SECTOR_SIZE);
    if(write_sector(secAddress+i, sectorBuffer) == ERROR) return ERROR;
  }
  setBlockAsUsed(blockAddress);
  free(sectorBuffer);
  return blockAddress;
}

/*
  Apaga um bloco do disco (Seta seu índice como vazio);
*/
int deleteBlock(int blockNumber){
  setBlockAsEmpty(blockNumber);
}

/*
  Procura pelo primeiro índice de bloco vazio
*/
int searchEmptyBlock(){
  int blockIndex = searchBitmap2(BITMAP_DADOS, EMPTY_BLOCK);
  if(blockIndex <= 0) return ERROR;
  return blockIndex;
}

/*
  Seta um índice de bloco como vazio
*/
int setBlockAsEmpty(int blockIndex){
  return setBlockStatus(blockIndex, EMPTY_BLOCK);
}

/*
  Seta um índice de bloco como em uso
*/
int setBlockAsUsed(int blockIndex){
  return setBlockStatus(blockIndex, BLOCK_IN_USE);
}

/*
  Seta o status de um índice de bloco (Vazio ou em uso)
*/
int setBlockStatus(int blockIndex, int status){
  if(setBitmap2(BITMAP_DADOS, blockIndex, status) == 0) return ERROR;
  return SUCCESS;
}

int blockIsEmpty(int blockIndex){
  if(checkBlockStatus(blockIndex) == EMPTY_BLOCK) return true;
  return false;
}

int checkBlockStatus(int blockIndex){
  return getBitmap2(BITMAP_DADOS, blockIndex);
}

/*
  Transforma um índice de bloco em um endereço de setor
*/
int parseBlockNumberToSectorNumber(blockNumber){
  return getBlockAreaAddress() + (blockNumber * getBlockSectorsNumber());
}

/*
  Retorna o tamanho (em bytes) de um bloco
*/
int blockSize(){
  return getBlockSectorsNumber() * SECTOR_SIZE;
}

/*
  Retorna o número de setores que formam um bloco
*/
int getBlockSectorsNumber(){
  WORD * sectorsNumber = malloc(sizeof(WORD));
  getBlockSize(sectorsNumber);
  int buffer = (*sectorsNumber);
  free(sectorsNumber);
  return buffer;
}

/*
  Retorna o setor em que se inicia a área de blocos do disco
*/
int getBlockAreaAddress(){
  if(BLOCK_AREA_SECTOR != 0) return BLOCK_AREA_SECTOR;
  WORD * superBlockSize = malloc(sizeof(WORD));
  WORD * inodeBitmapSize = malloc(sizeof(WORD));
  WORD * blocksBitmapSize = malloc(sizeof(WORD));
  WORD * inodesAreaSize = malloc(sizeof(WORD));
  getSuperBlockSize(superBlockSize);
  getFreeInodeBitmapSize(inodeBitmapSize);
  getFreeBlocksBitmapSize(blocksBitmapSize);
  getInodeAreaSize(inodesAreaSize);
  BLOCK_AREA_SECTOR = (*superBlockSize) + (*inodeBitmapSize) + (*blocksBitmapSize) + (*inodesAreaSize);
  free(superBlockSize);
  free(inodeBitmapSize);
  free(blocksBitmapSize);
  free(inodesAreaSize);
  return BLOCK_AREA_SECTOR;
}
