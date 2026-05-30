#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/bpatch.h"

static void test_bpatch_create_free(void) {
    bpatch_t* bp = bpatch_create();
    assert(bp != NULL);
    bpatch_free(bp);
    printf("PASS: test_bpatch_create_free\n");
}

static void test_bpatch_free_null(void) {
    bpatch_free(NULL);
    printf("PASS: test_bpatch_free_null\n");
}

static void test_bpatch_header_load_bad_magic(void) {
    unsigned char bad[32];
    memset(bad, 0, sizeof(bad));
    bpatch_header_t* h = bpatch_header_load(bad, sizeof(bad));
    assert(h == NULL);
    printf("PASS: test_bpatch_header_load_bad_magic\n");
}

int main(void) {
    test_bpatch_create_free();
    test_bpatch_free_null();
    test_bpatch_header_load_bad_magic();
    printf("All bpatch tests passed.\n");
    return 0;
}
