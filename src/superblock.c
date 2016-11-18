#include <stdio.h>
#include <string.h>
#include "../include/superblock.h"
#include "../include/apidisk.h"
#include "../include/t2fs.h"


int readSuperBlock(){
  struct t2fs_superbloco superBloco;
  unsigned char buffer[SECTOR_SIZE];
  if(read_sector(0, buffer) != 0){
    printf("ERRO DE LEITURA\n");
  }
  memcpy(&superBloco, &buffer, sizeof(struct t2fs_superbloco));
  printf("%c%c%c%c\n", superBloco.id[0], superBloco.id[1], superBloco.id[2], superBloco.id[3]);
}
