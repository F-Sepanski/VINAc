#include "directory.h"
#include <string.h>

void inicializar_diretorio(DiretorioArchive *dir)
{
  dir->total_membros = 0;
  for (int i = 0; i < MAX_MEMBROS; i++)
  {
    dir->membros[i].nome[0] = '\0';
    dir->membros[i].uid = 0;
    dir->membros[i].tamanho_original = 0;
    dir->membros[i].tamanho_disco = 0;
    dir->membros[i].data_modificacao = 0;
    dir->membros[i].ordem = 0;
    dir->membros[i].offset = 0;
  }
}

int adicionar_membro(DiretorioArchive *dir, const char *nome, int uid, long tamanho_original, long tamanho_disco, long data_modificacao, int ordem, long offset)
{
  if (dir->total_membros >= MAX_MEMBROS)
  {
    return -1; // Diretório cheio
  }

  MembroDir *novo_membro = &dir->membros[dir->total_membros];
  strncpy(novo_membro->nome, nome, MAX_NOME - 1);
  novo_membro->nome[MAX_NOME - 1] = '\0'; // Garantir terminação nula
  novo_membro->uid = uid;
  novo_membro->tamanho_original = tamanho_original;
  novo_membro->tamanho_disco = tamanho_disco;
  novo_membro->data_modificacao = data_modificacao;
  novo_membro->ordem = ordem;
  novo_membro->offset = offset;

  dir->total_membros++;
  return 0; // Sucesso
  return 0;
}

int salvar_diretorio(FILE *arquivo, const DiretorioArchive *dir)
{
  if (fseek(arquivo, 0, SEEK_SET) != 0)
  {
    return -1; // Erro ao posicionar no início do arquivo
  }

  if (fwrite(dir, sizeof(DiretorioArchive), 1, arquivo) != 1)
  {
    return -1; // Erro ao escrever o diretório no arquivo
  }

  return 0; // Sucesso
  return 0;
}

int ler_diretorio(FILE *arquivo, DiretorioArchive *dir)
{
  if (fseek(arquivo, 0, SEEK_SET) != 0)
  {
    return -1; // Erro ao posicionar no início do arquivo
  }

  if (fread(dir, sizeof(DiretorioArchive), 1, arquivo) != 1)
  {
    return -1; // Erro ao ler o diretório do arquivo
  }

  return 0; // Sucesso
}
