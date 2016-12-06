#include "../include/block.h"
#include "../include/t2fs.h"
#include "../include/record.h"
#include "../include/bool.h"
#include <math.h>
#include <string.h>


struct t2fs_record * searchBlockForRecord(unsigner char * block, char * path){
  int i;
  bool foundRecord = false;
  struct t2fs_record * recordBuffer;
  for(i = 0; i < blockSize()/recordSize(); i++){
    memcpy(recordBuffer, &(block[i*recordSize()]), recordSize());
    if(recordBuffer->TypeVal === TYPEVAL_INVALIDO) continue;
    if(strcmp(recordBuffer->name, path) == 0){
      return recordBuffer;
    }
  }
  return ERROR;
}

struct t2fs_record * searchSingleIndexBlockFor(unsigner char * indexBlock, char * path){
  int i;
  unsigned char * nextBlock;
  struct t2fs_record * recordBuffer;
  for(i = 0; i < blockSize()/sizeof(int); i++){
    readBlock(block[i*sizeof(int)], nextBlock);
    recordBuffer = searchBlockForRecord(nextBlock, path);
    if(recordBuffer != ERROR) return recordBuffer;
  }
  return ERROR;
}

struct t2fs_record * searchDoubleIndexBlockFor(unsigner char * block, char * path){
  int i;
  unsigned char * nextBlock;
  struct t2fs_record * recordBuffer;
  for(i = 0; i < blockSize()/sizeof(int); i++){
    readBlock(block[i*sizeof(int)], nextBlock);
    recordBuffer = searchSingleIndexBlockFor(nextBlock, path);
    if(recordBuffer != ERROR) return recordBuffer;
  }
  return ERROR;
}

/*
  retorna o número de registros que cabem dentro de um bloco
*/
int recordsPerBlock(){
  return floor(blockSize()/recordSize());
}
