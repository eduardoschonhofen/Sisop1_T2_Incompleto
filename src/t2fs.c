#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/apidisk.h"
#include "../include/t2fs.h"
#include "t2f2_aux.c"

#define MAX_CHAR_NOME 65



int identify2 (char *name, int size)
{
  iniciarT2FS();
  char *grupo = "Eduardo Osielski Schonhofen 00273732\n Felipe Fernandes Bastos 00279793\n Lucas Romagnoli 00\0";
  if(size < strlen(grupo)){
  printf("Erro");
  return -1;
}
strncpy(name, grupo, strlen(grupo)+1);

return 0;

}
FILE2 create2(char *filename)
{

  Registro registro[50000];
  iniciarT2FS();

  char teste[256] = "Olá Felipe tudo bem com voce?";

  char aux[1024];

  escreveCluster(5, teste);

  leCluster(5, aux);

  leDiretorio(2,&registro);
  puts(aux);

  return;



}

int delete2(char *filename);
FILE2 open2(char *filename);
int close2 (FILE2 handle)
{
  if(handle<0||handle>10)
    return;
  else
  {

    arquivos[handle].status=0;
  }

}
int read2(FILE2 handle, char *buffer, int size)
{
  char *aux;
  aux=(char*)malloc(size);
  if(handle<0||handle>10 || arquivos[handle].status==0)
    return 0;
  else
  {

    return;
  }

}
int write2(FILE2 handle, char *buffer, int size);
int truncate2(FILE2 handle);
int seek2 (FILE2 handle, unsigned int offset);
/*
int mkdir2 (char *caminho)
{
	char nomedir[MAX_CHAR_NOME];
	Registro novodir;
	buscaCaminhoDoArquivo(caminho, nomedir); // FALTA TERMINAR ESSA FUNÇÃO, PRA PODERMOS TESTAR <---------------------------------------------
	if(!existeDiretorioComNome(nomedir)) // Testa se já existe um diretorio nesse caminho com esse nome
	{
		novodir.firstCluster = buscaFATLivre();
		if(novodir.firstCluster > 0)
		{
			novodir.TypeVal = TYPEVAL_DIRETORIO;
			strcpy(novodir.name, nomedir);
			novodir.bytesFileSize = TAMANHO_BLOCO;		// colocação das infos do Registro do diretorio
			novodir.clustersFileSize = 1;
			escreveFAT(novodir.firstCluster, 0xFFFFFFFF);	// Marcando na FAT como unico cluster desse arquivo (pois os diretorios sao limitados a um cluster, eu acho)
			return 0;
		}
	}
	return -1;
}
*/

int rmdir2(char * pathname)
{
	//Navega ate o diretorio! Qualquer problema pra chegar lah, retorna erro
	//Essa é a parte mais crucial que ainda nao temos. fazemos isso e o resto se ajeita
	
	//testa se o diretorio esta vazio(exeto . e ..)! soh pode remover se o diretorio esta vazio
	
	//Libera o cluster do diretorio na FAT
	//Libera a entrada de diretorio no diretorio pai
	
	//Retorna zero
	
	return -1;
}

int chdir2(char *pathname)
{
	// Navega ate o diretorio. Retorna erro se não encontra
	// Abre ele
	// coloca em clusterAtual o cluster desse diretorio
	// retorna 0
	return -1;
}
int getcwd2(char *pathname, int size)
{
	// a partir do diretorio inicial
	// navega ate o diretorio atual, compilando os nomes de cada diretorio passado no caminho
	
	// Retorna erro se o pathname fica maior que a informacao passada pela variavel size
	return -1;
}
DIR2 opendir2(char *pathname)
{
	// navega ate diretorio
	// acha uma posicao vaiza no vetor de diretorios. se nao conseguir, retorna erro
	// pega informacoes do diretorio e coloca no vetor de diretorios abertos
	// coloca o ponteiroAtual do diretorio pra 0
	// retorna a posicao do vetor de diretorios abertos
	return -1;
}

int readdir2 (DIR2 handle, DIRENT2 *dentry)
{
	DWORD cluster = diretorios[handle].Register.firstCluster;
	Registro registro;
	if(!leEntradaDiretorio(handle, &registro);
	{
		strcpy(dentry.name, registro.name);
		dentry.fileType = registro.TypeVal;
		dentry.fileSize = registro.clustersFileSize * superBloco.SectorsPerCluster * SECTOR_SIZE;
		return 0;
	}
	return -1;
}

int closedir2(DIR2 handle)
{
	// busca no vetor de diretorios com o handle
	// retira ele do vetor;
	// acho que eh soh isso
	return -1;
}
int ln2(char *linkname, char *filename);
