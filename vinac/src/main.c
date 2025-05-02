#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/vinac.h"

int main(int argc, char *argv[])
{
  if (argc < 3) {
    printf("Uso: %s -ip|-p|-x <archive> [membro1 membro2 ...]\n", argv[0]);
    return 1;
  }

  // Garante que o arquivo de saída tenha extensão .vc
  char archive_name_with_ext[1100];
  const char *archive_name = argv[2];
  size_t len = strlen(archive_name);
  if (len >= 3 && strcmp(archive_name + len - 3, ".vc") == 0) {
    strncpy(archive_name_with_ext, archive_name, sizeof(archive_name_with_ext) - 1);
    archive_name_with_ext[sizeof(archive_name_with_ext) - 1] = '\0';
  } else {
    snprintf(archive_name_with_ext, sizeof(archive_name_with_ext), "%s.vc", archive_name);
  }

  if (strcmp(argv[1], "-ip") == 0 || strcmp(argv[1], "-p") == 0) {
    if (argc < 4) {
      printf("Uso: %s -ip <archive> [membro1 membro2 ...]\n", argv[0]);
      return 1;
    }
    FILE *archive = fopen(archive_name_with_ext, "wb+");
    if (!archive)
    {
      perror("Erro ao criar arquivo archive");
      return 1;
    }
    DiretorioArchive dir;
    dir.membros = NULL; // Prevent invalid free in inicializar_diretorio
    inicializar_diretorio(&dir);
    long offset = sizeof(int) + sizeof(MembroDir) * (argc - 3); // Ajuste: diretório dinâmico
    for (int i = 3; i < argc; i++)
    {
      unsigned char *buffer = NULL;
      long tamanho = 0;
      if (ler_arquivo(argv[i], &buffer, &tamanho) != 0)
      {
        fprintf(stderr, "Erro ao ler membro: %s\n", argv[i]);
        fclose(archive);
        liberar_diretorio(&dir);
        return 1;
      }
      fseek(archive, offset, SEEK_SET);
      if (fwrite(buffer, 1, tamanho, archive) != (size_t)tamanho)
      {
        fprintf(stderr, "Erro ao escrever membro no archive\n");
        free(buffer);
        fclose(archive);
        liberar_diretorio(&dir);
        return 1;
      }
      adicionar_membro(&dir, argv[i], 0, tamanho, tamanho, 0, i - 3, offset);
      offset += tamanho;
      free(buffer);
    }
    // DEBUG: print total_membros antes de salvar
    printf("[DEBUG] Total membros antes de salvar: %d\n", dir.total_membros);
    // Garante que o ponteiro está no início antes de salvar o diretório
    fseek(archive, 0, SEEK_SET);
    salvar_diretorio(archive, &dir);
    int membros_salvos = dir.total_membros; // Salva antes de liberar
    fclose(archive);
    liberar_diretorio(&dir);
    printf("Arquivo %s criado com %d membro(s).\n", archive_name_with_ext, membros_salvos);
    return 0;
  } else if (strcmp(argv[1], "-x") == 0) {
    FILE *archive = fopen(archive_name_with_ext, "rb");
    if (!archive) {
      perror("Erro ao abrir archive para extração");
      return 1;
    }
    DiretorioArchive dir;
    memset(&dir, 0, sizeof(DiretorioArchive)); // Garante inicialização
    if (ler_diretorio(archive, &dir) != 0) {
      fprintf(stderr, "Erro ao ler diretório do archive\n");
      fclose(archive);
      return 1;
    }
    const char **nomes = NULL;
    int n = 0;
    if (argc > 3) {
      nomes = (const char**)&argv[3];
      n = argc - 3;
    }
    extrair_arquivo(archive, &dir, nomes, n);
    fclose(archive);
    liberar_diretorio(&dir);
    printf("Extração concluída.\n");
    return 0;
  } else {
    printf("Opção não reconhecida.\n");
    return 1;
  }
}
