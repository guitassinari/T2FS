struct t2fs_record * searchBlockFor(unsigner char * block, char * path){


}

struct t2fs_record * searchSingleIndexBlockFor(unsigner char * block, char * path){
  //TODO : implementar
  //Se nao achar, retornar erro!
  return ERROR;
}

struct t2fs_record * searchDoubleIndexBlockFor(unsigner char * block, char * path){
  //TODO: Implemenetar
  int i;
  for(i = 0; i < recordsPerBlock(); i++){
    searchSingleIndexBlockFor();
  }
  //Se nao achar, retornar erro!
  return ERROR;
}
