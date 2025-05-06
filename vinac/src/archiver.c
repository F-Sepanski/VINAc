#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "../include/archiver.h"
#include "../include/directory.h"
#include "../include/member.h"

// Cria um novo arquivo de archive vazio
int archiver_create(const char *archive_name) {
    FILE *fp = fopen(archive_name, "wb");
    if (!fp) return -1;
    size_t count = 0;
    fwrite(&count, sizeof(size_t), 1, fp); // Inicializa diretório vazio
    fclose(fp);
    return 0;
}

// Remove o arquivo de archive (usado para testes)
int archiver_remove_archive(const char *archive_name) {
    return unlink(archive_name);
}

// Função auxiliar: lê o diretório do archive para a RAM
int read_directory(FILE *fp, Directory *dir) {
    rewind(fp);
    size_t count = 0;
    if (fread(&count, sizeof(size_t), 1, fp) != 1) {
        dir->count = 0;
        return 0; // Considere diretório vazio
    }
    dir->count = count;
    for (size_t i = 0; i < count; ++i) {
        if (fread(&dir->members[i], sizeof(Member), 1, fp) != 1) return -1;
    }
    return 0;
}

// Função auxiliar: grava o diretório da RAM para o archive
static int write_directory(FILE *fp, const Directory *dir) {
    rewind(fp);
    if (fwrite(&dir->count, sizeof(size_t), 1, fp) != 1) return -1;
    for (size_t i = 0; i < dir->count; ++i) {
        if (fwrite(&dir->members[i], sizeof(Member), 1, fp) != 1) return -1;
    }
    return 0;
}

// Adiciona um membro ao archive (substitui se já existir)
int archiver_add_member(const char *archive_name, const char *member_name, const char *data, size_t size) {
    FILE *fp = fopen(archive_name, "r+b");
    if (!fp) return -1;
    Directory dir;
    directory_init(&dir);
    read_directory(fp, &dir);
    // Cria o membro
    Member member;
    create_member(&member, member_name, data, size);
    member.disk_size = size; // Sem compressão
    member.offset = sizeof(size_t) + (dir.count + 1) * sizeof(Member);
    // Substitui se já existir
    int found = 0;
    for (size_t i = 0; i < dir.count; ++i) {
        if (strcmp(dir.members[i].name, member_name) == 0) {
            dir.members[i] = member;
            found = 1;
            break;
        }
    }
    if (!found) {
        dir.members[dir.count] = member;
        dir.members[dir.count].order = dir.count;
        dir.count++;
    }
    // Grava diretório
    write_directory(fp, &dir);
    // Grava dados do membro ao final
    fseek(fp, 0, SEEK_END);
    fwrite(data, 1, size, fp);
    fclose(fp);
    delete_member(&member);
    return 0;
}

// Extrai um membro do archive para a memória (aloca e retorna em output_data, output_size)
int archiver_extract_member(const char *archive_name, const char *member_name, char **output_data, size_t *output_size) {
    FILE *fp = fopen(archive_name, "rb");
    if (!fp) return -1;
    Directory dir;
    directory_init(&dir);
    read_directory(fp, &dir);
    int found = 0;
    for (size_t i = 0; i < dir.count; ++i) {
        if (strcmp(dir.members[i].name, member_name) == 0) {
            *output_size = dir.members[i].size;
            *output_data = malloc(*output_size + 1);
            fseek(fp, dir.members[i].offset, SEEK_SET);
            fread(*output_data, 1, *output_size, fp);
            (*output_data)[*output_size] = '\0';
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found ? 0 : -1;
}

// Remove um membro do archive
int archiver_remove_member(const char *archive_name, const char *member_name) {
    FILE *fp = fopen(archive_name, "r+b");
    if (!fp) return -1;
    Directory dir;
    directory_init(&dir);
    read_directory(fp, &dir);
    int found = 0;
    for (size_t i = 0; i < dir.count; ++i) {
        if (strcmp(dir.members[i].name, member_name) == 0) {
            // Remove do diretório
            for (size_t j = i; j < dir.count - 1; ++j) {
                dir.members[j] = dir.members[j + 1];
                dir.members[j].order = j;
            }
            dir.count--;
            found = 1;
            break;
        }
    }
    if (found) write_directory(fp, &dir);
    fclose(fp);
    return found ? 0 : -1;
}

// Move um membro no archive para imediatamente após o membro target (ou para o início se target_name for NULL)
int archiver_move_member(const char *archive_name, const char *member_name, const char *target_name) {
    FILE *fp = fopen(archive_name, "r+b");
    if (!fp) return -1;
    Directory dir;
    directory_init(&dir);
    read_directory(fp, &dir);
    directory_move_member(&dir, member_name, target_name);
    write_directory(fp, &dir);
    fclose(fp);
    return 0;
}
