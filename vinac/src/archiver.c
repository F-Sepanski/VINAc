#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include "archiver.h"
#include "../include/lz/lz.h"

void criar_archive(const char *nome_archive)
{
  FILE *archive = fopen(nome_archive, "wb");
  if (!archive)
  {
    perror("Erro ao criar archive");
    exit(1);
  }
  // Escreve um diretório vazio (apenas total_membros = 0)
  int total_membros = 0;
  fwrite(&total_membros, sizeof(int), 1, archive);
  fclose(archive);
}

void criar_diretorio_recursivo(const char *path) {
    char tmp[MAX_NOME];
    strncpy(tmp, path, MAX_NOME);
    tmp[MAX_NOME-1] = '\0';
    char *p = tmp;
    while (*p == '/') p++; // ignora barras iniciais
    for (; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(tmp, 0755);
            *p = '/';
        }
    }
}

void extrair_arquivo(FILE *arquivo, DiretorioArchive *dir, const char **nomes, int n)
{
    for (int i = 0; i < dir->total_membros; i++) {
        int extrair = 0;
        if (n == 0 || nomes == NULL) {
            extrair = 1;
        } else {
            for (int j = 0; j < n; j++) {
                if (strcmp(dir->membros[i].nome, nomes[j]) == 0) {
                    extrair = 1;
                    break;
                }
            }
        }
        if (extrair) {
            if (fseek(arquivo, dir->membros[i].offset, SEEK_SET) != 0)
                continue;
            long tam_comp = dir->membros[i].tamanho_disco;
            long tam_orig = dir->membros[i].tamanho_original;
            unsigned char *buffer_comp = malloc(tam_comp);
            unsigned char *buffer_orig = malloc(tam_orig);
            if (!buffer_comp || !buffer_orig) {
                free(buffer_comp);
                free(buffer_orig);
                continue;
            }
            if (fread(buffer_comp, 1, tam_comp, arquivo) != (size_t)tam_comp) {
                free(buffer_comp);
                free(buffer_orig);
                continue;
            }
            // Descomprime ou copia diretamente
            char nome_base[MAX_NOME];
            strncpy(nome_base, dir->membros[i].nome, MAX_NOME);
            nome_base[MAX_NOME-1] = '\0';
            char *base = basename(nome_base);
            FILE *out = fopen(base, "wb");
            if (!out) {
                fprintf(stderr, "Não foi possível criar arquivo: %s\n", base);
                free(buffer_comp);
                free(buffer_orig);
                continue;
            }
            if (dir->membros[i].comprimido) {
                // Descomprime
                LZ_Uncompress(buffer_comp, buffer_orig, (unsigned int)tam_comp);
                fwrite(buffer_orig, 1, tam_orig, out);
            } else {
                // Apenas copia o buffer
                fwrite(buffer_comp, 1, tam_orig, out);
            }
            fclose(out);
            free(buffer_comp);
            free(buffer_orig);
        }
    }
}
