#
# Makefile ESQUELETO
#
# DEVE ter uma regra "all" para geração da biblioteca
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#
#

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
TESTE_DIR=./teste

all: teste lib t2fs.o superblock.o

lib: t2fs.o superblock.o
	ar crs $(LIB_DIR)/libt2fs.a $(LIB_DIR)/t2fs.o $(LIB_DIR)/superblock.o $(LIB_DIR)/apidisk.o $(LIB_DIR)/bitmap2.o

t2fs.o:
	gcc -c -o $(LIB_DIR)/t2fs.o $(SRC_DIR)/t2fs.c

superblock.o:
	gcc -c -o $(LIB_DIR)/superblock.o $(SRC_DIR)/superblock.c

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o

superblockTeste: superblock.o
	gcc $(TESTE_DIR)/superblock.c $(LIB_DIR)/superblock.o $(LIB_DIR)/apidisk.o -o $(BIN_DIR)/superblock

teste1: lib
	gcc $(TESTE_DIR)/teste1.c $(LIB_DIR)/libt2fs.a -o $(BIN_DIR)/teste1

teste2: lib
	gcc $(TESTE_DIR)/teste2.c $(LIB_DIR)/libt2fs.a -o $(BIN_DIR)/teste2
