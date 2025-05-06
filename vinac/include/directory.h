#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stddef.h>
#include "member.h"

#define DIRECTORY_MAX_MEMBERS 128

// Estrutura que representa o diretório do archive
typedef struct {
    Member members[DIRECTORY_MAX_MEMBERS]; // Array de membros
    size_t count;                          // Quantidade de membros
} Directory;

// Inicializa o diretório vazio
void directory_init(Directory *dir);

// Adiciona um membro ao diretório (copia o membro)
void directory_add_member(Directory *dir, const Member *member);

// Remove um membro do diretório pelo nome
void directory_remove_member(Directory *dir, const char *name);

// Lista os membros do diretório
void directory_list_members(const Directory *dir);

// Libera recursos associados ao diretório
void directory_free(Directory *dir);

// Move um membro para imediatamente após o membro target (ou para o início se target_name for NULL)
void directory_move_member(Directory *dir, const char *member_name, const char *target_name);

#endif // DIRECTORY_H
