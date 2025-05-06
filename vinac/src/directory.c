#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/directory.h"

void directory_init(Directory *dir) {
    if (!dir) return;
    dir->count = 0;
}

void directory_add_member(Directory *dir, const Member *member) {
    if (!dir || !member || dir->count >= DIRECTORY_MAX_MEMBERS) return;
    // Substitui se já existir
    for (size_t i = 0; i < dir->count; ++i) {
        if (strcmp(dir->members[i].name, member->name) == 0) {
            delete_member(&dir->members[i]);
            dir->members[i] = *member;
            dir->members[i].order = i;
            dir->members[i].data = NULL; // Garante que não haverá free inválido
            return;
        }
    }
    dir->members[dir->count] = *member;
    dir->members[dir->count].order = dir->count;
    dir->members[dir->count].data = NULL; // Garante que não haverá free inválido
    dir->count++;
}

void directory_remove_member(Directory *dir, const char *name) {
    if (!dir || !name) return;
    for (size_t i = 0; i < dir->count; ++i) {
        if (strcmp(dir->members[i].name, name) == 0) {
            // Libera dados se necessário
            delete_member(&dir->members[i]);
            // Move os membros seguintes para trás
            for (size_t j = i; j < dir->count - 1; ++j) {
                dir->members[j] = dir->members[j + 1];
                dir->members[j].order = j;
            }
            dir->count--;
            break;
        }
    }
}

void directory_list_members(const Directory *dir) {
    if (!dir) return;
    printf("Ordem | Nome                | UID   | Tamanho Orig | Tamanho Disco | Data Modificação\n");
    for (size_t i = 0; i < dir->count; ++i) {
        const Member *m = &dir->members[i];
        char date_str[32];
        struct tm *tm_info = localtime(&m->mod_time);
        strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%5d | %-20s | %5d | %12zu | %13zu | %s\n",
            m->order, m->name, m->uid, m->size, m->disk_size, date_str);
    }
}

void directory_free(Directory *dir) {
    if (!dir) return;
    for (size_t i = 0; i < dir->count; ++i) {
        delete_member(&dir->members[i]);
    }
    dir->count = 0;
}

void directory_move_member(Directory *dir, const char *member_name, const char *target_name) {
    if (!dir || !member_name) return;
    size_t member_idx = dir->count, target_idx = dir->count;
    for (size_t i = 0; i < dir->count; ++i) {
        if (strcmp(dir->members[i].name, member_name) == 0) member_idx = i;
        if (target_name && strcmp(dir->members[i].name, target_name) == 0) target_idx = i;
    }
    if (member_idx == dir->count) return; // membro não encontrado
    Member temp = dir->members[member_idx];
    // Remove o membro
    for (size_t i = member_idx; i < dir->count - 1; ++i) {
        dir->members[i] = dir->members[i + 1];
    }
    // Insere após o target (ou no início se target_name for NULL)
    size_t insert_idx = (target_name == NULL) ? 0 : target_idx + 1;
    if (insert_idx > dir->count - 1) insert_idx = dir->count - 1;
    for (size_t i = dir->count - 1; i > insert_idx; --i) {
        dir->members[i] = dir->members[i - 1];
    }
    dir->members[insert_idx] = temp;
    // Atualiza ordem
    for (size_t i = 0; i < dir->count; ++i) {
        dir->members[i].order = i;
    }
}
