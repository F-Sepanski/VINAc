#ifndef MEMBER_H
#define MEMBER_H

#include <stddef.h>
#include <time.h>
#include <sys/types.h>

#define MEMBER_NAME_MAX 1024

// Estrutura que representa um membro do archive
typedef struct {
    char name[MEMBER_NAME_MAX]; // Nome do membro
    uid_t uid;                  // UID do proprietário
    size_t size;                // Tamanho original
    size_t disk_size;           // Tamanho em disco (pode ser diferente se comprimido)
    time_t mod_time;            // Data de modificação
    int order;                  // Ordem no arquivo
    size_t offset;              // Offset no archive
    char *data;                 // Ponteiro para os dados do membro (alocado dinamicamente)
} Member;

// Cria um membro a partir de dados em memória
void create_member(Member *member, const char *name, const char *data, size_t size);

// Lê os dados de um membro (retorna uma cópia dos dados, deve ser liberada pelo chamador)
char *read_member(const Member *member);

// Libera os recursos associados a um membro
void delete_member(Member *member);

#endif // MEMBER_H
