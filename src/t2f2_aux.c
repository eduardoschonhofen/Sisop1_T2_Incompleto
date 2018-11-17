#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int leCluster(DWORD cluster, BYTE *buffer)
{
	int n;
	DWORD setor = cluster * superbloco.SectorsPerCluster;
	for(n = 0; n < superbloco.SectorsPerCluster; n++)
	{
		if(read_sector(superbloco.DataSectorStart+setor + n, buffer + (n * SECTOR_SIZE))) // Acredito que sejam esses os parâmentros, mas tenho dúvidas
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
  puts("Start");
BYTE *buffer = (unsigned char*)malloc(sizeof(superbloco.SectorsPerCluster*SECTOR_SIZE));
printf("Buffer:%d",buffer);
leCluster(cluster,buffer);
registro->TypeVal=buffer[0];
strcpy(registro->name,buffer[1]);
registro->bytesFileSize=*((DWORD*)(buffer + 52));
registro->clustersFileSize=*((DWORD*)(buffer + 56));
registro->firstCluster=*((DWORD*)(buffer + 60));
free(buffer);
}
