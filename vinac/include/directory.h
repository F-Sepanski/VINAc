#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>

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
  int comprimido; // 0 = não comprimido, 1 = comprimido
} MembroDir;

typedef struct
{
  int total_membros;
  int capacidade;
  MembroDir *membros;
} DiretorioArchive;

// Inicializa o diretório vazio
DiretorioArchive *criar_diretorio();
void inicializar_diretorio(DiretorioArchive *dir);
void liberar_diretorio(DiretorioArchive *dir);

// Adiciona um membro ao diretório
int adicionar_membro(DiretorioArchive *dir, const char *nome, int uid, long tamanho_original, long tamanho_disco, long data_modificacao, int ordem, long offset);

// Salva o diretório no início do arquivo
int salvar_diretorio(FILE *arquivo, const DiretorioArchive *dir);

// Lê o diretório do início do arquivo
int ler_diretorio(FILE *arquivo, DiretorioArchive *dir);

#endif // DIRECTORY_H
