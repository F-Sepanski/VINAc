#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <stddef.h>
#include <stdio.h>
#include "directory.h"
#include "member.h"

// Cria um novo arquivo de archive vazio
int archiver_create(const char *archive_name);

// Adiciona um membro ao archive (substitui se já existir)
int archiver_add_member(const char *archive_name, const char *member_name, const char *data, size_t size);

// Extrai um membro do archive para a memória (aloca e retorna em output_data, output_size)
int archiver_extract_member(const char *archive_name, const char *member_name, char **output_data, size_t *output_size);

// Remove um membro do archive
int archiver_remove_member(const char *archive_name, const char *member_name);

// Move um membro no archive para imediatamente após o membro target (ou para o início se target_name for NULL)
int archiver_move_member(const char *archive_name, const char *member_name, const char *target_name);

// Remove o arquivo de archive (usado para testes)
int archiver_remove_archive(const char *archive_name);

// Torna read_directory visível para main.c
int read_directory(FILE *fp, Directory *dir);

#endif // ARCHIVER_H
