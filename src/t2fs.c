#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include "t2f2_aux.c"


int identify2 (char *name, int size);
{
  iniciarT2FS();
  char *grupo = "Eduardo Osielski Schonhofen 00273732\n Felipe Fernandes Bastos 00279793\n Lucas Romagnoli 00\0";
  if(size < strlen(grupo)){
  printf("Erro")
  return -1;
}
strncpy(name, group, strlen(grupo)+1);

return 0;

}
FILE2 create2(char *filename)
{
  iniciarT2FS();

  Registro registro;



}
int delete2(char *filename);
FILE2 open2(char *filename);
int close2 (FILE2 handle);
int read2(FILE2 handle, char *buffer, int size);
int write2(FILE2 handle, char *buffer, int size);
int truncate2(FILE2 handle);
int seek2 (FILE2 handle, unsigned int offset);
int mkdir2 (char *caminho)
{
	char nomedir[MAX_CHAR_NOME];
	t2fs_record novodir;
	buscaCaminhoDoArquivo(caminho, nomedir); // Separar o resto do caminho do nome do diretorio a ser criado
	if(não exite getDiretorio(nomedir, restoDoCaminho)) // Testa se já existe um diretorio nesse caminho com esse nome
	{
		if(achaClusterVazio(&novodir.firstCluster)) // retorna 0 caso não encontre um cluster pra usar, e 1 caso encontre
		{
			novodir.TypeVal = TYPEVAL_DIRETORIO;
			strcpy(novodir.name, nomedir);
			novodir.bytesFileSize = TAMANHO_BLOCO;		// colocação das infos do t2fs_record do diretorio
			novodir.clustersFileSize = 1;
			escreveFAT(novodir.firstCluster, 0xFFFFFFFF);	// Marcando na FAT como unico cluster desse arquivo (pois os diretorios sao limitados a um cluster, eu acho)
			return 0;
		}
	}
	return -1;
}
int rmdir2(char * pathname);
int chdir2(char *pathname);
int getcwd2(char *pathname, int size);
DIR2 opendir2(char *pathname);
int readdir2 (DIR2 handle, DIRENT@ *dentry);
int closedir2(DIR2 handle);
int ln2(char *linkname, char *filename);
