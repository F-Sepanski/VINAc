#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/archiver.h"
#include "../include/directory.h"
#include "../include/member.h"

void print_usage() {
    printf("Uso: vinac <opcao> <archive> [membro1 membro2 ...]\n");
    printf("Opcoes:\n");
    printf("  -ip, -p   : Insere/acrescenta membros sem compressao\n");
    printf("  -ic, -i   : Insere/acrescenta membros com compressao\n");
    printf("  -m membro target: Move membro para apos target (ou para o inicio se target for NULL)\n");
    printf("  -x        : Extrai membros indicados (ou todos se nao indicados)\n");
    printf("  -r        : Remove membros indicados\n");
    printf("  -c        : Lista o conteudo do archive\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }
    const char *option = argv[1];
    const char *archive = argv[2];

    if (strcmp(option, "-ip") == 0 || strcmp(option, "-p") == 0) {
        // Inserir membros sem compressao
        if (argc < 4) {
            printf("Nenhum membro especificado para inserir.\n");
            return 1;
        }
        // Cria archive se nao existir
        FILE *fp = fopen(archive, "rb");
        if (!fp) archiver_create(archive);
        else fclose(fp);
        for (int i = 3; i < argc; ++i) {
            FILE *mf = fopen(argv[i], "rb");
            if (!mf) {
                printf("Nao foi possivel abrir o membro: %s\n", argv[i]);
                continue;
            }
            fseek(mf, 0, SEEK_END);
            size_t size = ftell(mf);
            fseek(mf, 0, SEEK_SET);
            char *data = malloc(size);
            if (!data) {
                printf("Memoria insuficiente para ler %s\n", argv[i]);
                fclose(mf);
                continue;
            }
            fread(data, 1, size, mf);
            fclose(mf);
            // Usa apenas o nome do arquivo, sem path
            const char *slash = strrchr(argv[i], '/');
            const char *member_name = slash ? slash + 1 : argv[i];
            if (archiver_add_member(archive, member_name, data, size) == 0)
                printf("Membro %s inserido com sucesso.\n", member_name);
            else
                printf("Falha ao inserir membro %s.\n", member_name);
            free(data);
        }
    } else if (strcmp(option, "-r") == 0) {
        // Remover membros
        if (argc < 4) {
            printf("Nenhum membro especificado para remover.\n");
            return 1;
        }
        for (int i = 3; i < argc; ++i) {
            if (archiver_remove_member(archive, argv[i]) == 0)
                printf("Membro %s removido com sucesso.\n", argv[i]);
            else
                printf("Falha ao remover membro %s.\n", argv[i]);
        }
    } else if (strcmp(option, "-x") == 0) {
        // Extrair membros
        if (argc == 3) {
            // Extrair todos
            FILE *fp = fopen(archive, "rb");
            if (!fp) {
                printf("Nao foi possivel abrir o archive.\n");
                return 1;
            }
            Directory dir;
            directory_init(&dir);
            extern int read_directory(FILE*, Directory*); // hack: tornar visível
            read_directory(fp, &dir);
            for (size_t i = 0; i < dir.count; ++i) {
                char *data = NULL;
                size_t size = 0;
                if (archiver_extract_member(archive, dir.members[i].name, &data, &size) == 0) {
                    // Extrai sempre para a pasta atual
                    FILE *out = fopen(dir.members[i].name, "wb");
                    if (out) {
                        fwrite(data, 1, size, out);
                        fclose(out);
                        printf("Extraido: %s\n", dir.members[i].name);
                    }
                    free(data);
                }
            }
            fclose(fp);
        } else {
            // Extrair membros especificados
            for (int i = 3; i < argc; ++i) {
                // Usa apenas o nome do arquivo, sem path
                const char *slash = strrchr(argv[i], '/');
                const char *member_name = slash ? slash + 1 : argv[i];
                char *data = NULL;
                size_t size = 0;
                if (archiver_extract_member(archive, member_name, &data, &size) == 0) {
                    FILE *out = fopen(member_name, "wb");
                    if (out) {
                        fwrite(data, 1, size, out);
                        fclose(out);
                        printf("Extraido: %s\n", member_name);
                    }
                    free(data);
                } else {
                    printf("Falha ao extrair membro %s.\n", member_name);
                }
            }
        }
    } else if (strcmp(option, "-c") == 0) {
        // Listar conteudo do archive
        FILE *fp = fopen(archive, "rb");
        if (!fp) {
            printf("Nao foi possivel abrir o archive.\n");
            return 1;
        }
        Directory dir;
        directory_init(&dir);
        extern int read_directory(FILE*, Directory*); // hack: tornar visível
        read_directory(fp, &dir);
        directory_list_members(&dir);
        fclose(fp);
    } else if (strcmp(option, "-m") == 0) {
        // Mover membro
        if (argc < 5) {
            printf("Uso: vinac -m <archive> <membro> <target|NULL>\n");
            return 1;
        }
        const char *member = argv[3];
        const char *target = (strcmp(argv[4], "NULL") == 0) ? NULL : argv[4];
        if (archiver_move_member(archive, member, target) == 0)
            printf("Membro %s movido com sucesso.\n", member);
        else
            printf("Falha ao mover membro %s.\n", member);
    } else {
        print_usage();
        return 1;
    }
    return 0;
}
