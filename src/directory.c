#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../include/apidisk.h"
#include "../include/inode.h"
#include "../include/block.h"
#include "../include/t2fs.h"
#include "../include/bool.h"
#include "../include/bitmap2.h"
#include "../include/indexBlock.h"

extern char[MAX_FILE_NAME_SIZE] currentDir = '/';

//TODO: implementar HANDLES
//TODO: abrir e fechar diretórios? Como assim? Serão como arquivos abertos?

/*
  cria um novo diretório
*/
int createDirectory(char * path){
  char * dirPath = getDirectoryPath(path);
  struct t2fs_inode root = getRootInode();
  strtok() //pegar tokens do path
  searchInodeFor(root, token);
  //TODO: varrer subdiretorios até chegar ao diretorio desejado
  //TODO: retornar erro se algum subdiretorio não existir
}


searchInodeFor(struct t2fs_inode * inode, char * path){
  unsigned char * block;
  struct t2fs_record * record;
  if(inode->dataPtr[0] != INVALID_PTR){
    readBlock(inode->dataPtr[0], block);
    record = searchBlockFor(block, path);
  }

  if(record != ERROR) return record;
  if(inode->dataPtr[1] != INVALID_PTR){
    readBlock(inode->dataPtr[1], block);
    record = searchBlockFor(block, path);
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
}



/*
 ????
*/
int newDirectory(){

}

/*
  abre um novo diretório e o seta como diretório atual
  TODO; Basicamente basta colocar currentDir com o caminho do diretório desejado
*/
int openDirectory(){

}

/*
  fecha o diretório atual
  TODO: ???? oi ??
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
  retorna o tamanho, em BYTES, de um registro de diretório
*/
int recordSize(){
  return sizeof(struct t2fs_record);
}

/*
  retorna o número de registros que cabem dentro de um bloco
*/
int recordsPerBlock(){
  return floor(blockSize()/recordSize());
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
