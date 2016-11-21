#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include "../include/bool.h"

#define SUPERBLOCK_SECTOR 0

int readSuperBlock();
int getId(char * buffer);
int getVersion(WORD * buffer);
int getSuperBlockSize(WORD * buffer);
int getFreeBlocksBitmapSize(WORD * buffer);
int getFreeInodeBitmapSize(WORD * buffer);
int getInodeAreaSize(WORD * buffer);
int getBlockSize(WORD * buffer);
int getDiskSize(DWORD * buffer);
