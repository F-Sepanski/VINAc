// filepath: /vinac/test/test_directory.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory.h"

// Function prototypes for the test functions
void test_add_member();
void test_remove_member();
void test_list_members();

int main() {
    // Run the tests
    test_add_member();
    test_remove_member();
    test_list_members();

    printf("All tests completed.\n");
    return 0;
}

void test_add_member() {
    // Test adding a member to the directory
    // Initialize a directory structure and add a member
    Directory dir;
    init_directory(&dir);

    Member member;
    strcpy(member.name, "test_file.txt");
    member.size = 1024;
    member.uid = 1000;
    member.mod_time = time(NULL);

    add_member(&dir, &member);

    // Verify that the member was added
    if (dir.count == 1 && strcmp(dir.members[0].name, "test_file.txt") == 0) {
        printf("test_add_member passed.\n");
    } else {
        printf("test_add_member failed.\n");
    }

    // Clean up
    free_directory(&dir);
}

void test_remove_member() {
    // Test removing a member from the directory
    Directory dir;
    init_directory(&dir);

    Member member;
    strcpy(member.name, "test_file.txt");
    member.size = 1024;
    member.uid = 1000;
    member.mod_time = time(NULL);

    add_member(&dir, &member);
    remove_member(&dir, "test_file.txt");

    // Verify that the member was removed
    if (dir.count == 0) {
        printf("test_remove_member passed.\n");
    } else {
        printf("test_remove_member failed.\n");
    }

    // Clean up
    free_directory(&dir);
}

void test_list_members() {
    // Test listing members in the directory
    Directory dir;
    init_directory(&dir);

    Member member1, member2;
    strcpy(member1.name, "test_file1.txt");
    member1.size = 1024;
    member1.uid = 1000;
    member1.mod_time = time(NULL);

    strcpy(member2.name, "test_file2.txt");
    member2.size = 2048;
    member2.uid = 1001;
    member2.mod_time = time(NULL);

    add_member(&dir, &member1);
    add_member(&dir, &member2);

    // Capture the output of list_members
    printf("Listing members:\n");
    list_members(&dir);

    // Clean up
    free_directory(&dir);
}