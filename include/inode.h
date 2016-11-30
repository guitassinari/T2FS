#include "../include/t2fs.h"


int readInode(int inodeIndex, struct t2fs_inode * buffer);
int writeInode(struct t2fs_inode * buffer);
