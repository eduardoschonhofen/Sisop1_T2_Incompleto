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
  char *grupo = "Eduardo Osielski Schonhofen 00274732\n Felipe Fernandes Bastos 00279793\n Lucas Romagnoli 00194235\0";
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
  DWORD cluster = clusterFromPath(filename);
  if((int)cluster<0)
  {
  //novoArquivo(filename);
  return -1;
  }
  else
  {
  //zeraArquivo(cluster);
  return -1;
  }

  return -1;



}

int delete2(char *filename)
{
  return -1;
}
FILE2 open2(char *filename)
{
  return -1;
}
int close2 (FILE2 handle)
{
  if(handle<0||handle>10)
    return;
  else
  {

    arquivos[handle].status=0;
    return 0;
  }

}
int read2(FILE2 handle, char *buffer, int size)
{
  iniciarT2FS();
  char *aux;
  aux=(char*)malloc(size);
  if(handle<0||handle>10 || arquivos[handle].status==0)
    return -1;
  else
  {

    return -1;
  }

}
int write2(FILE2 handle, char *buffer, int size)
{
  return -1;
}
int truncate2(FILE2 handle)
{
  return -1;
}
int seek2 (FILE2 handle, unsigned int offset)
{
  return -1;
}

int mkdir2 (char *caminho)
{
  iniciarT2FS();
	char nomedir[MAX_CHAR_NOME];
	Registro novodir;
	char *aux;
	aux = (char*) malloc(sizeof(MAX_FILE_NAME_SIZE+1));
	strcpy(aux, caminho);
	trataPathName(aux);
	nomeDiretorioDoPath(aux, nomedir); // FALTA TERMINAR ESSA FUNÇÃO, PRA PODERMOS TESTAR <---------------------------------------------
	if(!existeDiretorioComNome(nomedir)) // Testa se já existe um diretorio nesse caminho com esse nome
	{
		novodir.firstCluster = buscaFATLivre();
		if(novodir.firstCluster > 0)
		{
			novodir.TypeVal = TYPEVAL_DIRETORIO;
			strcpy(novodir.name, nomedir);
			novodir.bytesFileSize = superbloco.SectorsPerCluster * SECTOR_SIZE;		// colocação das infos do Registro do diretorio
			novodir.clustersFileSize = 1;
			escreveFAT(novodir.firstCluster, 0xFFFFFFFF);	// Marcando na FAT como unico cluster desse arquivo (pois os diretorios sao limitados a um cluster, eu acho)
			return 0;
		}
	}
	return -1;
}


int rmdir2(char * pathname)
{
	//Navega ate o diretorio! Qualquer problema pra chegar lah, retorna erro
	//Essa é a parte mais crucial que ainda nao temos. fazemos isso e o resto se ajeita

	//testa se o diretorio esta vazio(exeto . e ..)! soh pode remover se o diretorio esta vazio

	//Libera o cluster do diretorio na FAT
	//Libera a entrada de diretorio no diretorio pai

	//Retorna zero
  iniciarT2FS();
	int i, j;
	char *aux, *buffer;
	aux = (char*) malloc(sizeof(MAX_FILE_NAME_SIZE+1));
	buffer = (char*)malloc(superbloco.SectorsPerCluster*SECTOR_SIZE*1024);
	strcpy(aux, pathname);
	trataPathName(aux);
	char pathDiretorioPai[MAX_FILE_NAME_SIZE+1], nomedir[MAX_FILE_NAME_SIZE+1], nometeste[50];
	DWORD clusterPai;
	if(diretorioEstaVazio(aux))
	{
		diretorioPai(aux, pathDiretorioPai); // FALTA <----------------------------
		nomeDiretorioDoPath(aux, nomedir);  // FALTA <----------------------------

		clusterPai = clusterFromPath(pathDiretorioPai); // FALTA <----------------------------

		if(clusterPai > 0)
		{
			leCluster(clusterPai,buffer);
			for(j=2;j<(SECTOR_SIZE*superbloco.SectorsPerCluster)/64;j++)
			{
				if(buffer[64*j] == TYPEVAL_DIRETORIO)
				{
					for(i=0;i<50;i++)
						nometeste[i]=buffer[(1+i)+(j*64)];
					if (!strcmp(nomedir, nometeste))
					{
						escreveFAT(*((DWORD*)(buffer + (60+(j*64)))), 0);
						buffer[64*j] = TYPEVAL_INVALIDO;
						*((DWORD*)(buffer + (52+(j*64)))) = 0; // CONFERIR <--------------------------------
						*((DWORD*)(buffer + (56+(j*64)))) = 0; // CONFERIR <--------------------------------
						*((DWORD*)(buffer + (60+(j*64)))) = 0; // CONFERIR <--------------------------------
						escreveCluster(clusterPai, buffer);
						free(buffer);
						free(aux);
						return 0;
					}
				}
			}
			free(buffer);
			free(aux);
		}
	}

	return -1;
}

int chdir2(char *pathname)
{
	// Navega ate o diretorio. Retorna erro se não encontra
	// Abre ele
	// coloca em clusterAtual o cluster desse diretorio
	// retorna 0
  iniciarT2FS();
	char *aux;
	aux = (char*) malloc(sizeof(MAX_FILE_NAME_SIZE+1));
	strcpy(aux, pathname);
	trataPathName(aux);
	DWORD cluster = clusterFromPath(aux);
	if(cluster < 0)
		return -1;
	clusterAtual = cluster;
	strcpy(currentPath, aux);
	return 0;
}
int getcwd2(char *pathname, int size)
{
  iniciarT2FS();
	// a partir do diretorio inicial
	// navega ate o diretorio atual, compilando os nomes de cada diretorio passado no caminho

	// Retorna erro se o pathname fica maior que a informacao passada pela variavel size
	if(strlen(currentPath) < size)
	{
		strcpy(pathname, currentPath);
		return 0;
	}

	return -1;
}
DIR2 opendir2(char *pathname)
{
	// navega ate diretorio
	// acha uma posicao vaiza no vetor de diretorios. se nao conseguir, retorna erro
	// pega informacoes do diretorio e coloca no vetor de diretorios abertos
	// coloca o ponteiroAtual do diretorio pra 0
	// retorna a posicao do vetor de diretorios abertos
	iniciarT2FS();
	char pathDiretorioPai[MAX_FILE_NAME_SIZE+1], nomedir[MAX_FILE_NAME_SIZE+1];
	char *aux;
	aux = (char*) malloc(sizeof(MAX_FILE_NAME_SIZE+1));
	strcpy(aux, pathname);
	trataPathName(aux);
  printf("%d",strlen(aux));
  puts("BBB");
  puts(aux);
  puts(pathDiretorioPai);
	diretorioPai(aux, pathDiretorioPai); // FALTA <----------------------------
puts("AAA");
  nomeDiretorioDoPath(aux, nomedir); // FALTA <----------------------------
printf("%d",strlen(aux));
  DWORD cluster = clusterFromPath(pathDiretorioPai); // FALTA <----------------------------
	DIR2 handle;
	Registro buffer;
  puts("AAA");
	if(cluster > 0)
	{
		handle = buscaHandleDirLivre();
    puts("CCC");
		if (handle >= 0)
		{
			leEntradaDiretorioPorNome(cluster, nomedir, &buffer);
			diretorios[handle].Register.TypeVal=buffer.TypeVal;
			strcpy(diretorios[handle].Register.name, nomedir);
			diretorios[handle].Register.bytesFileSize=buffer.bytesFileSize;
			diretorios[handle].Register.clustersFileSize=buffer.clustersFileSize;
			diretorios[handle].Register.firstCluster=buffer.firstCluster;
			diretorios[handle].status = OPEN;
			diretorios[handle].currentPointer = 0;
			return 0;
		}
	}
	return -1;
}

int readdir2 (DIR2 handle, DIRENT2 *dentry)
{
  iniciarT2FS();
	DWORD cluster = diretorios[handle].Register.firstCluster;
	Registro registro;
	if(!leEntradaDiretorio(handle, &registro))
	{
		strcpy(dentry->name, registro.name);
		dentry->fileType = registro.TypeVal;
		dentry->fileSize=registro.clustersFileSize*superbloco.SectorsPerCluster*SECTOR_SIZE;
		return 0;
	}
	return -1;
}

int closedir2(DIR2 handle)
{
  iniciarT2FS();
	if(handleDIRValido(handle))
	{
		diretorios[handle].status = CLOSED;
		return 0;
	}
	return -1;
}
int ln2(char *linkname, char *filename)
{
  iniciarT2FS();
  return -1;
}
