#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <stdio.h>
#include "directory.h"

// Tamanho reservado para o diretório no início do arquivo
#define DIR_SIZE sizeof(DiretorioArchive)

// Cria um arquivo .vc e inicializa o diretório
void criar_archive(const char *nome_archive);

// Extrai membros do archive para o disco.
// Se nomes==NULL ou n==0, extrai todos.
void extrair_arquivo(FILE *arquivo, DiretorioArchive *dir, const char **nomes, int n);

#endif // ARCHIVER_H
