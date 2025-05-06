// filepath: /vinac/test/test_member.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "member.h"

// Test function for creating a member
void test_create_member() {
    Member member;
    const char *name = "test_file.txt";
    const char *data = "This is a test file.";
    
    // Create member
    create_member(&member, name, data, strlen(data));
    
    // Validate member properties
    if (strcmp(member.name, name) != 0) {
        printf("Test failed: Member name mismatch.\n");
        return;
    }
    if (member.size != strlen(data)) {
        printf("Test failed: Member size mismatch.\n");
        return;
    }
    if (strcmp(member.data, data) != 0) {
        printf("Test failed: Member data mismatch.\n");
        return;
    }
    
    printf("test_create_member passed.\n");
    delete_member(&member);
}

// Test function for reading a member
void test_read_member() {
    Member member;
    const char *name = "test_file.txt";
    const char *data = "This is a test file.";
    
    // Create member
    create_member(&member, name, data, strlen(data));
    
    // Read member data
    char *read_data = read_member(&member);
    
    // Validate read data
    if (strcmp(read_data, data) != 0) {
        printf("Test failed: Read data mismatch.\n");
        free(read_data);
        return;
    }
    
    free(read_data);
    printf("test_read_member passed.\n");
    delete_member(&member);
}

// Test function for deleting a member
void test_delete_member() {
    Member member;
    const char *name = "test_file.txt";
    const char *data = "This is a test file.";
    
    // Create member
    create_member(&member, name, data, strlen(data));
    
    // Delete member
    delete_member(&member);
    
    // Validate member is deleted
    if (member.data != NULL) {
        printf("Test failed: Member not deleted properly.\n");
        return;
    }
    
    printf("test_delete_member passed.\n");
}

// Main function to run tests
int main() {
    test_create_member();
    test_read_member();
    test_delete_member();
    
    return 0;
}
