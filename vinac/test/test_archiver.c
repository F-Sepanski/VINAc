// filepath: /vinac/test/test_archiver.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/archiver.h"

void test_archiver_add_member() {
    const char *archive_name = "test_archive.vc";
    const char *member_name = "test_file.txt";
    const char *data = "This is a test file.";
    archiver_create(archive_name);
    int result = archiver_add_member(archive_name, member_name, data, strlen(data));
    if (result != 0) {
        printf("Failed to add member: %s\n", member_name);
    } else {
        printf("Successfully added member: %s\n", member_name);
    }
    archiver_remove_archive(archive_name);
}

void test_archiver_extract_member() {
    const char *archive_name = "test_archive.vc";
    const char *member_name = "test_file.txt";
    const char *data = "This is a test file.";
    archiver_create(archive_name);
    archiver_add_member(archive_name, member_name, data, strlen(data));
    char *output_data = NULL;
    size_t output_size = 0;
    int result = archiver_extract_member(archive_name, member_name, &output_data, &output_size);
    if (result != 0) {
        printf("Failed to extract member: %s\n", member_name);
    } else {
        printf("Successfully extracted member: %s\n", member_name);
        printf("Extracted data: %s\n", output_data);
        free(output_data);
    }
    archiver_remove_archive(archive_name);
}

void test_archiver_remove_member() {
    const char *archive_name = "test_archive.vc";
    const char *member_name = "test_file.txt";
    const char *data = "This is a test file.";
    archiver_create(archive_name);
    archiver_add_member(archive_name, member_name, data, strlen(data));
    int result = archiver_remove_member(archive_name, member_name);
    if (result != 0) {
        printf("Failed to remove member: %s\n", member_name);
    } else {
        printf("Successfully removed member: %s\n", member_name);
    }
    archiver_remove_archive(archive_name);
}

int main() {
    test_archiver_add_member();
    test_archiver_extract_member();
    test_archiver_remove_member();
    return 0;
}
