#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/apidisk.h"
#include "../include/inode.h"
#include "../include/block.h"
#include "../include/t2fs.h"
#include "../include/bool.h"
#include "../include/bitmap2.h"
#include "../include/indexBlock.h"
#include "../include/record.h"

extern char[MAX_FILE_NAME_SIZE] currentDir = '/';

/*
  cria um novo diretório
*/
int createDirectory(char * path){
  char * dirPath = getDirectoryPath(path); //transforma o caminho do diretorio em absoluto
  char * dirName = cutDirName(path); //captura o nome do diretorio a ser criado e o remove do caminho absoluto
  struct t2fs_inode root = getRootInode();
  struct t2fs_record * recordBuffer;
  char * subDirectoryName;
  subDirectoryName = strtok(dirPath,"/"); //Nome do primeiro subdiretorio
  recordBuffer = searchInodeFor(root, subDirectoryName);
  subDirectoryName = strtok(NULL,"/"); //Nome do segundo subdiretorio
  struct t2fs_inode inodeBuffer;
  while(subDirectoryName != NULL){ //Busca de todos os subdiretórios
    readInode(recordBuffer->inodeNumber, inodeBuffer);
    recordBuffer = searchInodeFor(inodeBuffer, subDirectoryName);
    if(recordBuffer == ERROR) return ERROR;
    subDirectoryName = strtok(NULL,"/");
  }
  if(recordBuffer->TypeVal == TYPEVAL_DIRETORIO){

  } else return ERROR;
}

char * cutDirName(char * dirPath){
  char * destiny = malloc(sizeof(char)*strlen(strrchr(dirPath, "/") + 1));
  memcpy(destiny, strrchr(dirPath, "/") + 1, sizeof(char)*strlen(strrchr(dirPath, "/") + 1));
  strrchr(dirPath, "/") = "\0"; //Remove o último '/' e o troca por fim de string, removendo o último nome de diretorio da string
  return destiny;
}

searchInodeFor(struct t2fs_inode * inode, char * path){
  unsigned char * block;
  struct t2fs_record * record;
  if(inode->dataPtr[0] != INVALID_PTR){
    readBlock(inode->dataPtr[0], block);
    record = searchBlockForRecord(block, path);
  }

  if(record != ERROR) return record;
  if(inode->dataPtr[1] != INVALID_PTR){
    readBlock(inode->dataPtr[1], block);
    record = searchBlockForRecord(block, path);
  }

  if(record != ERROR) return record;
  if(inode->singleIndPtr != INVALID_PTR){
    readBlock(inode->singleIndPtr, block);
    record = searchSingleIndexBlockFor(block, path);
  }

  if(record != ERROR) return record;
  if(inode->doubleIndPtr != INVALID_PTR){
    readBlock(inode->doubleIndPtr, block);
    int i;
    unsigned char * indexBlock;
    for(i = 0; i < (blockSize()/sizeof(int)); i++){
      readBlock(block[i*sizeof(int)], indexBlock);
      record = searchIndexBlockFor(indexBlock, path);
    }
  }

  return record;
}

/*
  abre um novo diretório e o seta como diretório atual
*/
int openDirectory(){

}

/*
  fecha o diretório atual
*/
int closeDirectory(){

}

/*
  retorna o inode referente ao diretório ROOT
*/
struct t2fs_inode * getRootInode(){
  struct t2fs_inode * rootInode = malloc(sizeof(struct t2fs_inode));
  if(readInode(ROOT_DIR_INODE, rootInode) == ERROR){
    free(rootInode);
    rootInode = createRoot();
  }
  return rootInode;
}

/*
  inicializa o inode ROOT juntamente com um bloco inicial com registros inicializados, mas vazios
*/
struct t2fs_inode * createRoot(){
  struct t2fs_inode * rootInode = newInode();
  unsigned char * block = newDirectoryBlock();
  int blockAddress = writeBlock(block);
  rootInode->dataPtr[0] = blockAddress;
  writeRootInode(rootInode);
  return rootInode;
}

/*
  retorna um novo bloco de diretório, contendo o número maximo de registros, todos vazios
*/
unsigned char * newDirectoryBlock(){
  unsigned char * block = malloc(blockSize());
  struct t2fs_record * emptyRecord = newRecord();
  int i;
  for(i = 0; i < recordsPerBlock(); i++){
    memcpy(&block[i*recordSize()], emptyRecord, recordSize());
  }
  free(emptyRecord);
  return block;
}

/*
  retorna um ponteiro para um registro vazio
*/
struct t2fs_record * newRecord(){
  struct t2fs_record * record = malloc(recordSize());
  record->TypeVal = TYPEVAL_INVALIDO;
  record->name = "";
  record->blocksFileSize = 0;
  record->bytesFileSize = 0;
  record->inodeNumber = INVALID_PTR;
  return record;
}

/*
  transforma o caminho de um diretório, seja ele relativo ou absoluto, em um caminho absoluto a partir do diretório ROOT
*/
char * getDirectoryPath(char * dirPath){
  char * fullPath = malloc(MAX_FILE_NAME_SIZE);
  if(isAbsolutePath(path)) fullPath = strcpy(fullPath, path);
  else {
    fullPath = strcpy(fullPath, currentDir);
    fullPath = strcat(fullPath, path);
  }
  return fullPath;
}

/*
  testa se o caminha passado é absoluto, ou seja, começa a partir do diretório ROOT
*/
bool isAbsolutePath(char * path){
  return path[0] == '/';
}
