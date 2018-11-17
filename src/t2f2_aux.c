#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/apidisk.h"
#include "../include/t2fs.h"

#define MAX_ARQUIVOS_ABERTOS 10
#define MAX_DIRETORIOS_ABERTOS 10
#define FALSE 0
#define TRUE 1

struct t2fs_info{
  DWORD SectorPerCluster;
  DWORD FATSectorStart;
  DWORD RootDirCluster;
  DWORD DataSectorStart;
  DWORD numClusters; // NofSectors - DataSectorStart
  DWORD FATEntriesPerSector; // SECTOR_SIZE / sizeof(DWORD)
  DWORD clusterSize; // SectorPerCluster * SECTOR_SIZE
  DWORD fileEntriesPerCluster; // ClusterSize /sizeof(struct registro)
};


typedef struct t2fs_superbloco superBloco;
typedef struct t2fs_record Registro;
struct t2fs_info bloco= {};

typedef struct t2fs_openfile{
  Registro register;
} arquivosAbertos;




//Variavveis globais //
arquivosAbertos  arquivos[MAX_ARQUIVOS_ABERTOS];
arquivosAbertos  diretorios[MAX_DIRETORIOS_ABERTOS];
bool inicializouT2FS = FALSE;
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

  strcpy(currentPath, "/\0");
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


return 0;

}

void inicializaBloco()
{
  bloco.SectorPerCluster = superbloco.SectorPerCluster;
  bloco.FATSectorStart = superbloco.pFATSectorStart;
  bloco.RootDirCluster = superbloco.RootDirCluster;
  bloco.DataSectorStart = superbloco.DataSectorStart;
  bloco.numClusters = (superbloco.NofSectors - bloco.DataSectorStart)/superbloco.SectorPerCluster;
  bloco.FATEntriesPerSector = SECTOR_SIZE / sizeof(DWORD);
  bloco.clusterSize = bloco.SectorPerCluster * SECTOR_SIZE;
  bloco.fileEntriesPerCluster = (bloco.clusterSize / sizeof( Registro ));

}

void iniciarArquivosAbertos()
{
  int i;
  for(i=0; i <MAX_ARQUIVOS_ABERTOS; i++){
      arquivos[i].register.TypeVal = TYPEVAL_INVALIDO;

  }
}


void iniciarDiretoriosAbertos()
{
int i;
for(i=0; i <MAX_DIRETORIOS_ABERTOS; i++){
    diretorios[i].register.TypeVal = TYPEVAL_INVALIDO;
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
    if(arquivos[freeHandle].register.TypeVal = TYPEVAL_INVALIDO)
      return freeHandle;
  }
  return -1;
}

DIR2 buscaHandleDirLivre()
{
  DIR2  freeHandle;
  for(freeHandle = 0; freeHandle < MAX_DIRETORIOS_ABERTOS;freeHandle++)
  {
    if(diretorios[freeHandle].register.TypeVal = TYPEVAL_INVALIDO)
      return freeHandle;
  }
  return -1;
}


BOOL handleFileValido(FILE2 handle)
{
  if(handle < 0 || handle >= MAX_ARQUIVOS_ABERTOS || arquivos[handle].register.TypeVal != TYPEVAL_REGULAR)
      return FALSE;
  else
      return TRUE;
}

BOOL handleDIRValido(DIR2 handle)
{
  if(handle < 0 || handle >= MAX_DIRETORIOS_ABERTOS || diretorios[handle].register.TypeVal != TYPEVAL_DIRETORIO)
      return FALSE;
  else
      return TRUE;
}

int leCluster(DWORD cluster, BYTE *buffer)
{
	int n;
	DWORD setor = cluster * superbloco.SectorsPerCluster;
	for(n = 0; n < superbloco.SectorsPerCluster; n++)
	{
		if(read_sector(setor + n, buffer + (n * SECTOR_SIZE))) // Acredito que sejam esses os parâmentros, mas tenho dúvidas
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
		if(write_sector(setor + n, buffer + (n * SECTOR_SIZE))) // Acredito que sejam esses os parâmentros, mas tenho dúvidas
			return -1;
	}
	return 0;
}
