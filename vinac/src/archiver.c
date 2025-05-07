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

// Função auxiliar: reescreve todo o archive a partir do diretório e dos dados dos membros
static int rewrite_archive(const char *archive_name, Directory *dir, const Member *new_member, const char *new_data, size_t new_data_size, int replace_idx, int insert_idx) {
    (void)new_member;
    (void)insert_idx;
    char temp_name[1100];
    snprintf(temp_name, sizeof(temp_name), "%s.tmp", archive_name);
    FILE *fpw = fopen(temp_name, "wb+");
    if (!fpw) return -1;
    size_t dir_offset = sizeof(size_t) + dir->count * sizeof(Member);
    size_t curr_offset = dir_offset;
    // Atualiza offsets dos membros
    for (size_t i = 0; i < dir->count; ++i) {
        dir->members[i].offset = curr_offset;
        curr_offset += dir->members[i].disk_size;
    }
    fwrite(&dir->count, sizeof(size_t), 1, fpw);
    fwrite(dir->members, sizeof(Member), dir->count, fpw);
    for (size_t i = 0; i < dir->count; ++i) {
        if ((int)i == replace_idx && new_data && new_data_size > 0) {
            // Escreve o novo conteúdo do membro atualizado
            fwrite(new_data, 1, new_data_size, fpw);
        } else {
            FILE *fpr = fopen(archive_name, "rb");
            if (!fpr) {
                fclose(fpw);
                remove(temp_name);
                return -1;
            }
            Directory tmp_dir;
            directory_init(&tmp_dir);
            read_directory(fpr, &tmp_dir);
            size_t j;
            for (j = 0; j < tmp_dir.count; ++j) {
                if (strcmp(tmp_dir.members[j].name, dir->members[i].name) == 0) break;
            }
            if (j < tmp_dir.count) {
                char *data = malloc(tmp_dir.members[j].disk_size);
                fseek(fpr, tmp_dir.members[j].offset, SEEK_SET);
                fread(data, 1, tmp_dir.members[j].disk_size, fpr);
                fwrite(data, 1, tmp_dir.members[j].disk_size, fpw);
                free(data);
            }
            fclose(fpr);
        }
    }
    fclose(fpw);
    remove(archive_name);
    rename(temp_name, archive_name);
    return 0;
}

// Adiciona um membro ao archive (substitui se já existir)
int archiver_add_member(const char *archive_name, const char *member_name, const char *data, size_t size) {
    FILE *fp = fopen(archive_name, "rb");
    Directory dir;
    directory_init(&dir);
    if (fp) {
        read_directory(fp, &dir);
        fclose(fp);
    }
    int found = 0, found_idx = 0;
    for (size_t i = 0; i < dir.count; ++i) {
        if (strcmp(dir.members[i].name, member_name) == 0) {
            found = 1;
            found_idx = i;
            break;
        }
    }
    Member new_member;
    create_member(&new_member, member_name, data, size);
    new_member.disk_size = size;
    new_member.size = size;
    new_member.uid = getuid();
    new_member.mod_time = time(NULL);
    if (found) {
        // Atualiza o membro mantendo a ordem original
        int original_order = dir.members[found_idx].order;
        dir.members[found_idx] = new_member;
        dir.members[found_idx].order = original_order;
    } else {
        dir.members[dir.count] = new_member;
        dir.members[dir.count].order = dir.count;
        dir.count++;
    }
    // Após qualquer modificação, atualiza o campo order de todos os membros para garantir unicidade e consistência
    for (size_t i = 0; i < dir.count; ++i) {
        dir.members[i].order = i;
    }
    int ret = rewrite_archive(archive_name, &dir, &new_member, data, size, found ? found_idx : -1, found ? found_idx : (int)dir.count-1);
    delete_member(&new_member);
    return ret;
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
    FILE *fp = fopen(archive_name, "rb");
    if (!fp) return -1;
    Directory dir;
    directory_init(&dir);
    read_directory(fp, &dir);
    fclose(fp);
    int found = 0, remove_idx = -1;
    for (size_t i = 0; i < dir.count; ++i) {
        if (strcmp(dir.members[i].name, member_name) == 0) {
            found = 1;
            remove_idx = i;
            break;
        }
    }
    if (!found) return -1;
    for (size_t i = remove_idx; i < dir.count - 1; ++i) {
        dir.members[i] = dir.members[i + 1];
        dir.members[i].order = i;
    }
    dir.count--;
    // Reescreve archive sem o membro removido
    return rewrite_archive(archive_name, &dir, NULL, NULL, 0, -1, -1);
}

// Move um membro no archive para imediatamente após o membro target (ou para o início se target_name for NULL)
int archiver_move_member(const char *archive_name, const char *member_name, const char *target_name) {
    FILE *fp = fopen(archive_name, "rb");
    if (!fp) return -1;
    Directory dir;
    directory_init(&dir);
    read_directory(fp, &dir);
    fclose(fp);
    directory_move_member(&dir, member_name, target_name);
    // Reescreve archive com nova ordem
    return rewrite_archive(archive_name, &dir, NULL, NULL, 0, -1, -1);
}
