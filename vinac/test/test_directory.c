// filepath: /vinac/test/test_directory.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory.h"
#include "member.h"

// Function prototypes for the test functions
void test_directory_add_member();
void test_directory_remove_member();
void test_directory_list_members();

int main() {
    // Run the tests
    test_directory_add_member();
    test_directory_remove_member();
    test_directory_list_members();

    printf("All tests completed.\n");
    return 0;
}

void test_directory_add_member() {
    // Test adding a member to the directory
    Directory dir;
    directory_init(&dir);

    const char *name = "test_file.txt";
    const char *data = "conteudo de teste";

    Member member;
    create_member(&member, name, data, strlen(data));

    directory_add_member(&dir, &member);

    // Verify that the member was added
    if (dir.count == 1 && strcmp(dir.members[0].name, name) == 0 && dir.members[0].size == strlen(data)) {
        printf("test_directory_add_member passed.\n");
    } else {
        printf("test_directory_add_member failed.\n");
    }

    // Clean up
    directory_free(&dir);
    delete_member(&member);
}

void test_directory_remove_member() {
    // Test removing a member from the directory
    Directory dir;
    directory_init(&dir);

    const char *name = "test_file.txt";
    const char *data = "conteudo de teste";

    Member member;
    create_member(&member, name, data, strlen(data));

    directory_add_member(&dir, &member);
    directory_remove_member(&dir, name);

    // Verify that the member was removed
    if (dir.count == 0) {
        printf("test_directory_remove_member passed.\n");
    } else {
        printf("test_directory_remove_member failed.\n");
    }

    // Clean up
    directory_free(&dir);
    delete_member(&member);
}

void test_directory_list_members() {
    // Test listing members in the directory
    Directory dir;
    directory_init(&dir);

    const char *name1 = "test_file1.txt";
    const char *data1 = "conteudo1";
    const char *name2 = "test_file2.txt";
    const char *data2 = "conteudo2";

    Member member1, member2;
    create_member(&member1, name1, data1, strlen(data1));
    create_member(&member2, name2, data2, strlen(data2));

    directory_add_member(&dir, &member1);
    directory_add_member(&dir, &member2);

    // Capture the output of list_members
    printf("Listing members:\n");
    directory_list_members(&dir);

    // Clean up
    directory_free(&dir);
    delete_member(&member1);
    delete_member(&member2);
}
