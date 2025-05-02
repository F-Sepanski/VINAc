#include "directory.h"
#include <string.h>
#include <stdlib.h>

#define CAPACIDADE_INICIAL 8

DiretorioArchive *criar_diretorio() {
    DiretorioArchive *dir = (DiretorioArchive *)malloc(sizeof(DiretorioArchive));
    if (!dir) return NULL;
    dir->total_membros = 0;
    dir->capacidade = CAPACIDADE_INICIAL;
    dir->membros = (MembroDir *)malloc(sizeof(MembroDir) * dir->capacidade);
    if (!dir->membros) {
        free(dir);
        return NULL;
    }
    return dir;
}

void inicializar_diretorio(DiretorioArchive *dir)
{
    if (!dir) return;
    dir->total_membros = 0;
    dir->capacidade = CAPACIDADE_INICIAL;
    if (dir->membros != NULL) {
        free(dir->membros);
        dir->membros = NULL;
    }
    dir->membros = (MembroDir *)calloc(dir->capacidade, sizeof(MembroDir));
}

void liberar_diretorio(DiretorioArchive *dir) {
    if (dir->membros != NULL) {
        free(dir->membros);
        dir->membros = NULL;
    }
    dir->total_membros = 0;
    dir->capacidade = 0;
}

int adicionar_membro(DiretorioArchive *dir, const char *nome, int uid, long tamanho_original, long tamanho_disco, long data_modificacao, int ordem, long offset)
{
    if (dir->total_membros >= dir->capacidade) {
        int nova_capacidade = dir->capacidade * 2;
        MembroDir *novos = (MembroDir *)realloc(dir->membros, sizeof(MembroDir) * nova_capacidade);
        if (!novos) return -1;
        dir->membros = novos;
        dir->capacidade = nova_capacidade;
    }
    MembroDir *novo_membro = &dir->membros[dir->total_membros];
    strncpy(novo_membro->nome, nome, MAX_NOME - 1);
    novo_membro->nome[MAX_NOME - 1] = '\0';
    novo_membro->uid = uid;
    novo_membro->tamanho_original = tamanho_original;
    novo_membro->tamanho_disco = tamanho_disco;
    novo_membro->data_modificacao = data_modificacao;
    novo_membro->ordem = ordem;
    novo_membro->offset = offset;
    dir->total_membros++;
    return 0;
}

int salvar_diretorio(FILE *arquivo, const DiretorioArchive *dir)
{
    if (fseek(arquivo, 0, SEEK_SET) != 0)
        return -1;
    if (fwrite(&dir->total_membros, sizeof(int), 1, arquivo) != 1)
        return -1;
    if (fwrite(dir->membros, sizeof(MembroDir), dir->total_membros, arquivo) != (size_t)dir->total_membros)
        return -1;
    return 0;
}

int ler_diretorio(FILE *arquivo, DiretorioArchive *dir)
{
    if (fseek(arquivo, 0, SEEK_SET) != 0)
        return -1;
    int total = 0;
    if (fread(&total, sizeof(int), 1, arquivo) != 1)
        return -1;
    if (dir->membros != NULL) {
        free(dir->membros);
        dir->membros = NULL;
    }
    dir->membros = (MembroDir *)malloc(sizeof(MembroDir) * total);
    if (!dir->membros) return -1;
    dir->total_membros = total;
    dir->capacidade = total > 0 ? total : CAPACIDADE_INICIAL;
    if (fread(dir->membros, sizeof(MembroDir), total, arquivo) != (size_t)total)
        return -1;
    return 0;
}
