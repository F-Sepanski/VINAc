#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include "archiver.h"

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
            long tam = dir->membros[i].tamanho_disco;
            unsigned char *buffer = malloc(tam);
            if (!buffer) continue;
            if (fread(buffer, 1, tam, arquivo) != (size_t)tam) {
                free(buffer);
                continue;
            }
            // Extrai apenas o nome base do arquivo
            char nome_base[MAX_NOME];
            strncpy(nome_base, dir->membros[i].nome, MAX_NOME);
            nome_base[MAX_NOME-1] = '\0';
            char *base = basename(nome_base);
            FILE *out = fopen(base, "wb");
            if (!out) {
                fprintf(stderr, "Não foi possível criar arquivo: %s\n", base);
                free(buffer);
                continue;
            }
            fwrite(buffer, 1, tam, out);
            fclose(out);
            free(buffer);
        }
    }
}
