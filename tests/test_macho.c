#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/macho.h"

static void test_macho_create_free(void) {
    macho_t* m = macho_create();
    assert(m != NULL);
    assert(m->header == NULL);
    macho_free(m);
    printf("PASS: test_macho_create_free\n");
}

static void test_macho_free_null(void) {
    macho_free(NULL);
    printf("PASS: test_macho_free_null\n");
}

int main(void) {
    test_macho_create_free();
    test_macho_free_null();
    printf("All macho tests passed.\n");
    return 0;
}
