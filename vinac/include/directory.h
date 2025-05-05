#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_MEMBROS 128
#define MAX_NOME 1024

typedef struct
{
  char nome[MAX_NOME];
  int uid;
  long tamanho_original;
  long tamanho_disco;
  long data_modificacao;
  int ordem;
  long offset;
} MembroDir;

typedef struct
{
  int total_membros;
  MembroDir membros[MAX_MEMBROS];
} DiretorioArchive;

// Inicializa o diretório vazio
void inicializar_diretorio(DiretorioArchive *dir);

// Adiciona um membro ao diretório
int adicionar_membro(DiretorioArchive *dir, const char *nome, int uid, long tamanho_original, long tamanho_disco, long data_modificacao, int ordem, long offset);

// Salva o diretório no início do arquivo
int salvar_diretorio(FILE *arquivo, const DiretorioArchive *dir);

// Lê o diretório do início do arquivo
int ler_diretorio(FILE *arquivo, DiretorioArchive *dir);

#endif // DIRECTORY_H
