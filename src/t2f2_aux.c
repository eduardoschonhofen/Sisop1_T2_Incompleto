#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "../include/apidisk.h"
#include "../include/t2fs.h"

#define MAX_ARQUIVOS_ABERTOS 10
#define MAX_DIRETORIOS_ABERTOS 10
#define FALSE 0
#define TRUE 1


#define EOF 4294967295
#define FREE 0
#define INVALID 1
#define BADSECTOR 4294967294
struct t2fs_info{
  DWORD SectorsPerCluster;
  DWORD FATSectorStart;
  DWORD RootDirCluster;
  DWORD DataSectorStart;
  DWORD numClusters; // NofSectors - DataSectorStart
  DWORD FATEntriesPerSector; // SECTOR_SIZE / sizeof(DWORD)
  DWORD clusterSize; // SectorsPerCluster * SECTOR_SIZE
  DWORD fileEntriesPerCluster; // ClusterSize /sizeof(struct registro)
};


typedef struct t2fs_superbloco superBloco;
typedef struct t2fs_record Registro;
struct t2fs_info bloco= {};

struct t2fs_openfile{
  struct t2fs_record Register;
  int currentPointer;
  int status; //OPEN OR CLOSED
};
typedef struct t2fs_openfile arquivosAbertos;



//Variavveis globais //
arquivosAbertos  arquivos[MAX_ARQUIVOS_ABERTOS];
arquivosAbertos  diretorios[MAX_DIRETORIOS_ABERTOS];
int inicializouT2FS = FALSE;
DWORD clusterAtual;
char currentPath[MAX_FILE_NAME_SIZE+1];
superBloco superbloco;

int iniciarT2FS(){

  if (inicializouT2FS){
      return 0;
    }
  if (leituraSuperBloco() !=0)
  {
    printf("Erro");
  }

  inicializaBloco();
  iniciarArquivosAbertos();
  iniciarDiretoriosAbertos();
  clusterAtual = superbloco.RootDirCluster;

  strcpy(currentPath, "/");
  inicializouT2FS = TRUE;


}

int leituraSuperBloco()
{
  unsigned char buffer[SECTOR_SIZE];

  if(read_sector(0, buffer) != 0){
      return 1;
  }


  strncpy(superbloco.id,(char*)buffer , 4);
  superbloco.version = *((WORD*)(buffer + 4));
  superbloco.superblockSize = *((WORD*)(buffer + 6));
  superbloco.DiskSize = *((DWORD*)(buffer + 8));
  superbloco.NofSectors = *((DWORD*)(buffer + 12));
  superbloco.SectorsPerCluster = *((DWORD*)(buffer + 16));
  superbloco.pFATSectorStart = *((DWORD*)(buffer + 20));
  superbloco.RootDirCluster = *((DWORD*)(buffer + 24));
  superbloco.DataSectorStart= *((DWORD*)(buffer + 28));

printf("Id: %s\n", superbloco.id);
printf("Version: %d\n", superbloco.version);
printf("SuperBlock Size (Blocks): %d\n", superbloco.superblockSize);
printf("DiskSize : %d\n", superbloco.DiskSize);
printf("NofSectors: %d\n", superbloco.NofSectors);
printf("SectorsPerCluster: %d\n", superbloco.SectorsPerCluster);
printf("pFATSectorStart: %d\n", superbloco.pFATSectorStart);
printf("RootDirCluster: %d\n", superbloco.RootDirCluster);
printf("DataSectorStart: %d\n", superbloco.DataSectorStart);
return 0;

}

void inicializaBloco()
{
  bloco.SectorsPerCluster = superbloco.SectorsPerCluster;
  bloco.FATSectorStart = superbloco.pFATSectorStart;
  bloco.RootDirCluster = superbloco.RootDirCluster;
  bloco.DataSectorStart = superbloco.DataSectorStart;
  bloco.numClusters = (superbloco.NofSectors - bloco.DataSectorStart)/superbloco.SectorsPerCluster;
  bloco.FATEntriesPerSector = SECTOR_SIZE / sizeof(DWORD);
  bloco.clusterSize = bloco.SectorsPerCluster * SECTOR_SIZE;
  bloco.fileEntriesPerCluster = (bloco.clusterSize / sizeof( Registro ));

  printf("\n SectorsPerCluster: %d\n", bloco.SectorsPerCluster);
  printf("pFATSectorStart: %d\n", bloco.FATSectorStart);
  printf("RootDirCluster: %d\n", bloco.RootDirCluster);
  printf("DataSectorStart: %d\n", bloco.DataSectorStart);
  printf("numClusters: %d\n", bloco.numClusters);
  printf("FATEntriesPerSector: %d\n", bloco.FATEntriesPerSector);
  printf("clusterSize: %d\n", bloco.clusterSize);
  printf("fileEntriesPerCluster: %d\n", bloco.fileEntriesPerCluster);

}

void iniciarArquivosAbertos()
{
  int i;
  for(i=0; i <MAX_ARQUIVOS_ABERTOS; i++){
      arquivos[i].Register.TypeVal = TYPEVAL_INVALIDO;

  }
}


void iniciarDiretoriosAbertos()
{
int i;
for(i=0; i <MAX_DIRETORIOS_ABERTOS; i++){
    diretorios[i].Register.TypeVal = TYPEVAL_INVALIDO;
  }
}

void buscaCaminhoDoArquivo(char *pathname, char *filename)
{
  if(pathname[0]== "/") // caminho absoluto
  {
    DWORD guardaCluster;
    guardaCluster = clusterAtual;
    int i, tamanhoDoCaminho = strlen(pathname);

    for(i = 0;i < tamanhoDoCaminho; i++)
    {
        currentPath[i];
    }

  }
}

FILE2 buscaHandleArquivoLivre()
{
  FILE2  freeHandle;
  for(freeHandle = 0; freeHandle < MAX_ARQUIVOS_ABERTOS;freeHandle++)
  {
    if(arquivos[freeHandle].Register.TypeVal = TYPEVAL_INVALIDO)
      return freeHandle;
  }
  return -1;
}

DIR2 buscaHandleDirLivre()
{
  DIR2  freeHandle;
  for(freeHandle = 0; freeHandle < MAX_DIRETORIOS_ABERTOS;freeHandle++)
  {
    if(diretorios[freeHandle].Register.TypeVal = TYPEVAL_INVALIDO)
      return freeHandle;
  }
  return -1;
}


int handleFileValido(FILE2 handle)
{
  if(handle < 0 || handle >= MAX_ARQUIVOS_ABERTOS || arquivos[handle].Register.TypeVal != TYPEVAL_REGULAR)
      return FALSE;
  else
      return TRUE;
}

int handleDIRValido(DIR2 handle)
{
  if(handle < 0 || handle >= MAX_DIRETORIOS_ABERTOS || diretorios[handle].Register.TypeVal != TYPEVAL_DIRETORIO)
      return FALSE;
  else
      return TRUE;
}

int leCluster(DWORD cluster, char *buffer)
{
	int n;
	DWORD setor = cluster * superbloco.SectorsPerCluster;

	for(n = 0; n < superbloco.SectorsPerCluster; n++)
	{

		if(read_sector(superbloco.DataSectorStart+setor + n, buffer + (n * SECTOR_SIZE)))
			return -1;
	}
	return 0;
}

int escreveCluster(DWORD cluster, BYTE *buffer)
{
	int n;
	DWORD setor = cluster * superbloco.SectorsPerCluster;
	for(n = 0; n < superbloco.SectorsPerCluster; n++)
	{
		if(write_sector(superbloco.DataSectorStart+setor + n, buffer + (n * SECTOR_SIZE))) // Acredito que sejam esses os parâmentros, mas tenho dúvidas
			return -1;
	}
	return 0;
}
int leFAT(DWORD cluster, DWORD *buffer)
{
	int n;
  BYTE *setor=(BYTE*)malloc(SECTOR_SIZE);
	DWORD fatAdress =(int) cluster/bloco.FATEntriesPerSector;
  if(read_sector(superbloco.pFATSectorStart+fatAdress, setor)) // Acredito que sejam esses os parâmentros, mas tenho dúvidas
    return -1;
  DWORD adress=cluster%bloco.FATEntriesPerSector;
  memcpy(buffer,setor+adress,32);
  free(setor);
	return 0;
}


int escreveFAT(DWORD cluster, DWORD *buffer)
{
	int n;
  BYTE *setor=(BYTE*)malloc(SECTOR_SIZE);
	DWORD fatAdress =(int) cluster/bloco.FATEntriesPerSector;
  if(read_sector(superbloco.pFATSectorStart+fatAdress, setor)) // Acredito que sejam esses os parâmentros, mas tenho dúvidas
    return -1;
  DWORD adress=cluster%bloco.FATEntriesPerSector;
  memcpy(setor+adress,buffer,32);
  if(write_sector(superbloco.pFATSectorStart+fatAdress, setor))
  return -1;
  free(setor);
	return 0;
}


void leDiretorio(DWORD cluster,Registro* registro)
{
char *buffer;
int i;
int j;
buffer = (char*)malloc(superbloco.SectorsPerCluster*SECTOR_SIZE*1024);
if(buffer!=0)
{
leCluster(cluster,buffer);

for(j=0;j<(SECTOR_SIZE*superbloco.SectorsPerCluster)/64;j++)
{
registro[j].TypeVal=buffer[64*j];
for(i=0;i<50;i++)
registro[j].name[i]=buffer[(1+i)+(j*64)];
registro[j].bytesFileSize=*((DWORD*)(buffer + (52+(j*64))));
registro[j].clustersFileSize=*((DWORD*)(buffer + (56+(j*64))));
registro[j].firstCluster=*((DWORD*)(buffer + (60+(j*64))));

printf("TypeVal:%d\n",registro[j].TypeVal);
printf("Name:%s\n",registro[j].name);
printf("bytesFileSize:%d\n",registro[j].bytesFileSize);
printf("clustersFileSize:%d\n",registro[j].clustersFileSize);
printf("firstCluster:%d\n",registro[j].firstCluster);

}
free(buffer);



}
}

// Retorna a entrada livre da FAT ou -1 caso não encontre
DWORD buscaFATLivre()
{
	unsigned int n;
	DWORD buffer;
	int numPosicoesFAT = (superbloco.DataSectorStart - superbloco.pFATSectorStart) * superbloco.SectorsPerCluster * SECTOR_SIZE / 4; // calcula o num de bytes da fat, divide por quatro pro num de entradas da fat
	for(n = 2; n < numPosicoesFAT; n++)
	{
		if(leFAT(n, &buffer))
			return -1;
		if(buffer == 0)
			return n;
	}
	return -2;
}

int leEntradaDiretorio(DIR2 handle, Registro* registro)
{
	char *buffer;
	int i;

	buffer = (char*)malloc(superbloco.SectorsPerCluster*SECTOR_SIZE*1024);
	DWORD cluster = diretorios[handle].Register.firstCluster;
	int ponteiro = diretorios[handle].currentPointer;
	if(buffer!=0)
	{
		leCluster(cluster,buffer);
		registro.TypeVal=buffer[64*ponteiro];
		if (registro.TypeVal != 0)
		{
			for(i=0;i<50;i++)
				registro.name[i]=buffer[(1+i)+(ponteiro*64)];
			registro.bytesFileSize=*((DWORD*)(buffer + (52+(ponteiro*64))));
			registro.clustersFileSize=*((DWORD*)(buffer + (56+(ponteiro*64))));
			registro.firstCluster=*((DWORD*)(buffer + (60+(ponteiro*64))));
			diretorios[handle].currentPointer = diretorios[handle].currentPointer + 1; // atualiza current pointer
			free(buffer);
			return 0;
		}
	}
	return -1;

}

// retorna 0 se não existe diretorio com o nome especificado, se existe retorna 1
int existeDiretorioComNome(char* nome)
{
	char *buffer;
	int i;
	int j;
	buffer = (char*)malloc(superbloco.SectorsPerCluster*SECTOR_SIZE*1024);
	if(buffer!=0)
	{
		leCluster(clusterAtual,buffer);

		for(j=0;j<(SECTOR_SIZE*superbloco.SectorsPerCluster)/64;j++)
		{
			if(buffer[64*j] == TYPEVAL_DIRETORIO && !strcmp(buffer + 1 + j*64, nome))
			{
				free(buffer);
				return 1;
			}
		}
		free(buffer);
		return 0;
	}
	return -1;
}

// retorna 0 se diretorio  nao esta vazio, se nao esta vazio retorna 1
int diretorioEstaVazio(char *path)
{
	char *buffer;
	int i;
	int j;
	buffer = (char*)malloc(superbloco.SectorsPerCluster*SECTOR_SIZE*1024);
	DWORD cluster = clusterFromPath(path); // FALTA <----------------------------------------------------
	if(buffer!=0)
	{
		if (cluster > 0)
		{
			leCluster(cluster,buffer);

			for(j=2;j<(SECTOR_SIZE*superbloco.SectorsPerCluster)/64;j++) // Começa em 2 para pular '.' e '..', sempre presentes
			{
				if(buffer[64*j] != TYPEVAL_INVALIDO)
				{
					free(buffer);
					return 0;
				}
			}
			free(buffer);
			return 1;
		}
	}
	return -1;
}

int leEntradaDiretorioPorNome(DWORD cluster, char* nome, Registro* registro)
{
	char *buffer, nometeste[50];
	int i;

	buffer = (char*)malloc(superbloco.SectorsPerCluster*SECTOR_SIZE*1024);

	if(buffer!=0)
	{
		{
			leCluster(cluster,buffer);
			for(j=2;j<(SECTOR_SIZE*superbloco.SectorsPerCluster)/64;j++)
			{
				if(buffer[64*j] == TYPEVAL_DIRETORIO)
				{
					for(i=0;i<50;i++)
						nometeste[i]=buffer[(1+i)+(j*64)];
					if (!strcmp(nome, nometeste))
					{
						registro.TypeVal=buffer[64*j];
						strcpy(registro.name, nometeste);
						registro.bytesFileSize=*((DWORD*)(buffer + (52+(j*64))));
						registro.clustersFileSize=*((DWORD*)(buffer + (56+(j*64))));
						registro.firstCluster=*((DWORD*)(buffer + (60+(j*64))));
						return 0;
					}
				}
			}
			free(buffer);
		}
	}
	return -1;

}

int diretorioPai(char *pathname, char *pathDiretorioPai)
{
    char *aux, *diretorioPai;
    Registro *bufferRegistro;
    int i, tamanhoPath, ultimaBarra;
    tamanhoPath = strlen(pathname);
    bufferRegistro = (Registro*)malloc(sizeof(Registro));
    aux = (char*)malloc(sizeof(tamanhoPath));
    diretorioPai = (char*)malloc(sizeof(tamanhoPath));
    strcpy(aux, pathname);

    if(aux[0] == '/') //absoluto
    {
      for(i = 1;i < tamanhoPath + 1; i++)
      {
        if(aux[i] == '/')
        {
          ultimaBarra = i;
        }
      }
      for(i = 0, i < ultimaBarra, i++)
      {
        diretorioPai[i] = aux[i];
      }
      diretorioPai[i] = "\0"

      strcpy(pathDiretorioPai, diretorioPai);
    }

    else if(aux[0] == '.')
    {
      char pontoPonto[3] = "..";

      leEntradaDiretorioPorNome(clusterAtual,pontoPonto,bufferRegistro);


    }

}

DWORD clusterFromPath(char *path)
{
  int i, tamanhoPath, sucesso = 1;
  char *aux;
  DWORD clusterAux = superbloco.RootDirCluster;
  tamanhoPath = strlen(path);
  aux = (char*) malloc(sizeof(tamanhoPath));
  Registro *bufferRegistro;
  bufferRegistro = (Registro*)malloc(sizeof(Registro));
  i = 1;
  while(sucesso)
  {
    while(path[i] != '/' && path[i] != "\0")
    {
      aux[j] = path[i];
      i++;
      j++;
    }
    j = 0;
    if(path[i] == "\0")
    {
      sucesso = 0;
    }
    if(leEntradaDiretorioPorNome(clusterAux,aux,bufferRegistro) == -1)
    {
      return -1;
    }
    else
    {
      clusterAux = bufferRegistro.firstCluster;
    }
  }
return clusterAux;
}

int trataPathName(char *path)
{
  char *aux;
  aux = (char*) malloc(sizeof(MAX_FILE_NAME_SIZE+1));
  aux2=(char*) malloc(sizeof(MAX_FILE_NAME_SIZE+1));
  strcpy(aux,path);

  if(aux[0] == '/')
  {
    return 0;
  }

  else if(aux[0] == '.')
  {
  strcpy(aux2,currentPath);
  strcat(aux2,path+1);
  }
}
