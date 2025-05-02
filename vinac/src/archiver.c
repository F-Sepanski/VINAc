#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archiver.h"

void criar_archive(const char *nome_archive)
{
  FILE *archive = fopen(nome_archive, "wb");
  if (!archive)
  {
    perror("Erro ao criar archive");
    exit(1);
  }

  // Inicializa a área de diretório com zeros
  char dir_area[DIR_SIZE] = {0};
  fwrite(dir_area, 1, DIR_SIZE, archive);

  fclose(archive);
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
            // Posiciona no offset do membro
            if (fseek(arquivo, dir->membros[i].offset, SEEK_SET) != 0)
                continue;
            long tam = dir->membros[i].tamanho_disco;
            unsigned char *buffer = malloc(tam);
            if (!buffer) continue;
            if (fread(buffer, 1, tam, arquivo) != (size_t)tam) {
                free(buffer);
                continue;
            }
            FILE *out = fopen(dir->membros[i].nome, "wb");
            if (!out) {
                free(buffer);
                continue;
            }
            fwrite(buffer, 1, tam, out);
            fclose(out);
            free(buffer);
        }
    }
}
