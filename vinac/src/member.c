#include "member.h"

int ler_arquivo(const char *nome, unsigned char **buffer, long *tamanho)
{
  FILE *arquivo = fopen(nome, "rb");
  if (!arquivo)
    return -1;

  fseek(arquivo, 0, SEEK_END);
  *tamanho = ftell(arquivo);
  rewind(arquivo);

  *buffer = (unsigned char *)malloc(*tamanho);
  if (!*buffer)
  {
    fclose(arquivo);
    return -1;
  }

  size_t tamanho_ = (size_t)(*tamanho);
  if (fread(*buffer, 1, tamanho_, arquivo) != tamanho_)
  {
    free(*buffer);
    fclose(arquivo);
    return -1;
  }

  fclose(arquivo);
  return 0;
}
