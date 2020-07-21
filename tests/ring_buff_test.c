/*
 * File:   ring_buff_test.c
 * Author: kilnaar
 *
 * Created on Jul 20, 2020, 8:02:35 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "../ring_buff.h"

/*
 * CUnit Test Suite
 */
int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void it_inits() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, &buff, len);

    CU_ASSERT(ring_buff.sink_iteration == 0);
    CU_ASSERT(ring_buff.source_iteration == 0);
    CU_ASSERT(ring_buff.sink_pos == 0);
    CU_ASSERT(ring_buff.source_pos == 0);
}

void it_inits_empty() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, &buff, len);

    CU_ASSERT(ring_is_emtpy(&ring_buff));
}

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("ring_buff_test", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "it_inits", it_inits)) ||
            (NULL == CU_add_test(pSuite, "it_inits_empty", it_inits_empty))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
