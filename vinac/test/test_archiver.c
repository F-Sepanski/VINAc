// filepath: /vinac/test/test_archiver.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/archiver.h"

void test_add_member() {
    // Test adding a member to the archive
    // Setup
    const char *archive_name = "test_archive.vc";
    const char *member_name = "test_file.txt";
    const char *data = "This is a test file.";
    
    // Create an archive
    create_archive(archive_name);
    
    // Add a member
    int result = add_member(archive_name, member_name, data, strlen(data));
    if (result != 0) {
        printf("Failed to add member: %s\n", member_name);
    } else {
        printf("Successfully added member: %s\n", member_name);
    }
    
    // Cleanup
    remove_archive(archive_name);
}

void test_extract_member() {
    // Test extracting a member from the archive
    const char *archive_name = "test_archive.vc";
    const char *member_name = "test_file.txt";
    const char *data = "This is a test file.";
    
    // Create an archive and add a member
    create_archive(archive_name);
    add_member(archive_name, member_name, data, strlen(data));
    
    // Extract the member
    char *output_data = NULL;
    size_t output_size = 0;
    int result = extract_member(archive_name, member_name, &output_data, &output_size);
    if (result != 0) {
        printf("Failed to extract member: %s\n", member_name);
    } else {
        printf("Successfully extracted member: %s\n", member_name);
        printf("Extracted data: %s\n", output_data);
        free(output_data);
    }
    
    // Cleanup
    remove_archive(archive_name);
}

void test_remove_member() {
    // Test removing a member from the archive
    const char *archive_name = "test_archive.vc";
    const char *member_name = "test_file.txt";
    const char *data = "This is a test file.";
    
    // Create an archive and add a member
    create_archive(archive_name);
    add_member(archive_name, member_name, data, strlen(data));
    
    // Remove the member
    int result = remove_member(archive_name, member_name);
    if (result != 0) {
        printf("Failed to remove member: %s\n", member_name);
    } else {
        printf("Successfully removed member: %s\n", member_name);
    }
    
    // Cleanup
    remove_archive(archive_name);
}

int main() {
    test_add_member();
    test_extract_member();
    test_remove_member();
    
    return 0;
}