#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/backup.h"
#include "../src/backup_file.h"

static void test_backup_open_missing(void) {
    backup_t* b = backup_open("/nonexistent/path/xyz", "AABBCCDD");
    assert(b == NULL);
    printf("PASS: test_backup_open_missing\n");
}

static void test_backup_get_file_null(void) {
    backup_file_t* f = backup_get_file(NULL, "MediaDomain", "Media/test.txt");
    assert(f == NULL);
    printf("PASS: test_backup_get_file_null\n");
}

int main(void) {
    test_backup_open_missing();
    test_backup_get_file_null();
    printf("All backup tests passed.\n");
    return 0;
}
