#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/mbdb_record.h"

static void test_mbdb_record_create_free(void) {
    mbdb_record_t* r = mbdb_record_create();
    assert(r != NULL);
    mbdb_record_free(r);
    printf("PASS: test_mbdb_record_create_free\n");
}

static void test_mbdb_record_free_null(void) {
    mbdb_record_free(NULL);
    printf("PASS: test_mbdb_record_free_null\n");
}

int main(void) {
    test_mbdb_record_create_free();
    test_mbdb_record_free_null();
    printf("All mbdb_record tests passed.\n");
    return 0;
}
