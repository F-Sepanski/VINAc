#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/member.h"

void create_member(Member *member, const char *name, const char *data, size_t size) {
    if (!member || !name || !data) return;
    memset(member, 0, sizeof(Member)); // Zera toda a struct para evitar lixo/padding
    strncpy(member->name, name, MEMBER_NAME_MAX - 1);
    member->name[MEMBER_NAME_MAX - 1] = '\0';
    member->size = size;
    member->disk_size = size;
    member->uid = getuid();
    member->mod_time = time(NULL);
    member->order = 0;
    member->offset = 0;
    member->data = (char *)malloc(size + 1);
    if (member->data) {
        memcpy(member->data, data, size);
        member->data[size] = '\0';
    }
}

char *read_member(const Member *member) {
    if (!member || !member->data) return NULL;
    char *copy = (char *)malloc(member->size + 1);
    if (copy) {
        memcpy(copy, member->data, member->size);
        copy[member->size] = '\0';
    }
    return copy;
}

void delete_member(Member *member) {
    if (!member) return;
    if (member->data) {
        free(member->data);
        member->data = NULL;
    }
}
