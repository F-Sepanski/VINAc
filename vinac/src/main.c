#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/vinac.h"
#include "../include/lz/lz.h"

int main(int argc, char *argv[])
{
  if (argc < 3) {
    printf("Uso: %s -ip|-p|-x|-c <archive> [membro1 membro2 ...]\n", argv[0]);
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
      // --- Compressão LZ (Fast) ---
      unsigned char *comp_buffer = malloc(tamanho * 2 + 1); // espaço extra para compressão
      unsigned int *work = malloc(sizeof(unsigned int) * (tamanho + 65536));
      if (!comp_buffer || !work) {
        fprintf(stderr, "Erro de memória para compressão\n");
        free(buffer);
        free(comp_buffer);
        free(work);
        fclose(archive);
        liberar_diretorio(&dir);
        return 1;
      }
      int usar_comprimido = 0;
      int comp_size = LZ_CompressFast(buffer, comp_buffer, (unsigned int)tamanho, work);
      if (comp_size > 0 && comp_size < tamanho) {
        usar_comprimido = 1;
      }
      unsigned char *to_write = usar_comprimido ? comp_buffer : buffer;
      long size_to_write = usar_comprimido ? comp_size : tamanho;
      fseek(archive, offset, SEEK_SET);
      if (fwrite(to_write, 1, size_to_write, archive) != (size_t)size_to_write)
      {
        fprintf(stderr, "Erro ao escrever membro no archive\n");
        free(buffer);
        free(comp_buffer);
        free(work);
        fclose(archive);
        liberar_diretorio(&dir);
        return 1;
      }
      adicionar_membro(&dir, argv[i], 0, tamanho, size_to_write, 0, i - 3, offset);
      dir.membros[dir.total_membros-1].comprimido = usar_comprimido;
      offset += size_to_write;
      free(buffer);
      free(comp_buffer);
      free(work);
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
  } else if (strcmp(argv[1], "-c") == 0) {
    FILE *archive = fopen(archive_name_with_ext, "rb");
    if (!archive) {
      perror("Erro ao abrir archive para listagem");
      return 1;
    }
    DiretorioArchive dir;
    memset(&dir, 0, sizeof(DiretorioArchive));
    if (ler_diretorio(archive, &dir) != 0) {
      fprintf(stderr, "Erro ao ler diretório do archive\n");
      fclose(archive);
      return 1;
    }
    printf("%-4s %-30s %-6s %-12s %-12s %-20s %-6s\n", "ORD", "NOME", "UID", "TAM_ORIG", "TAM_DISCO", "DATA_MODIF", "COMP");
    for (int i = 0; i < dir.total_membros; i++) {
      MembroDir *m = &dir.membros[i];
      printf("%-4d %-30s %-6d %-12ld %-12ld %-20ld %-6s\n", m->ordem, m->nome, m->uid, m->tamanho_original, m->tamanho_disco, m->data_modificacao, m->comprimido ? "sim" : "nao");
    }
    fclose(archive);
    liberar_diretorio(&dir);
    return 0;
  } else {
    printf("Opção não reconhecida.\n");
    return 1;
  }
}
