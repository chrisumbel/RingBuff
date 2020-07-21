/*
Copyright 2020 Christopher Umbel

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
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
    ring_init(&ring_buff, buff, len);

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


void it_emtpies_after_sinked_bytes_read() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, buff, len);

    ring_write(&ring_buff, 0x55);
    ring_read(&ring_buff);
    
    CU_ASSERT(ring_is_emtpy(&ring_buff));
}

void it_emtpies_after_sinked_bytes_read_after_iteration() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, buff, len);

    ring_write(&ring_buff, 0x00);
    ring_write(&ring_buff, 0x01);
    ring_write(&ring_buff, 0x02);
    ring_write(&ring_buff, 0x03);
    ring_read(&ring_buff);
    ring_write(&ring_buff, 0x04);
    ring_read(&ring_buff);
    ring_read(&ring_buff);    
    ring_read(&ring_buff);    
    ring_read(&ring_buff);
    
    CU_ASSERT(ring_is_emtpy(&ring_buff));
}

void it_sources_one() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, buff, len);

    ring_write(&ring_buff, 0x55);
    CU_ASSERT(ring_buff.source_iteration == 0);
    CU_ASSERT(ring_buff.source_pos == 1);
    CU_ASSERT(ring_buff.sink_iteration == 0);
    CU_ASSERT(ring_buff.sink_pos == 0);
}

void it_sinks_one() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, buff, len);

    ring_write(&ring_buff, 0x55);
    unsigned int read = ring_read(&ring_buff);
    CU_ASSERT(read == 0x55);
    CU_ASSERT(ring_buff.source_iteration == 0);
    CU_ASSERT(ring_buff.source_pos == 1);
    CU_ASSERT(ring_buff.sink_iteration == 0);
    CU_ASSERT(ring_buff.sink_pos == 1);
}

void it_iterates_source() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, buff, len);
    
    ring_write(&ring_buff, 0x00);
    ring_write(&ring_buff, 0x01);
    ring_write(&ring_buff, 0x02);
    ring_write(&ring_buff, 0x03);
    CU_ASSERT(ring_buff.source_iteration == 1);
}

void it_iterates_sink() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, buff, len);
    
    ring_write(&ring_buff, 0x00);
    ring_write(&ring_buff, 0x01);
    ring_write(&ring_buff, 0x02);
    ring_write(&ring_buff, 0x03);
    CU_ASSERT(ring_buff.source_iteration == 1);
    CU_ASSERT(ring_buff.sink_iteration == 0);
    ring_read(&ring_buff);
    ring_write(&ring_buff, 0x04);
    ring_read(&ring_buff);
    ring_read(&ring_buff);
    CU_ASSERT(ring_buff.sink_iteration == 0);
    ring_read(&ring_buff);
    int read = ring_read(&ring_buff);
    CU_ASSERT(ring_buff.sink_iteration == 1);
    CU_ASSERT(read == 0x04);
}

void it_doesnt_write_past_sink_after_source_iterates() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, buff, len);
    
    ring_write(&ring_buff, 0x00);
    ring_write(&ring_buff, 0x01);
    ring_write(&ring_buff, 0x02);
    ring_write(&ring_buff, 0x03);
    CU_ASSERT(ring_buff.source_iteration == 1);
    int wrote = ring_write(&ring_buff, 0x04);
    CU_ASSERT(wrote == 0);
    CU_ASSERT(ring_buff.source_pos == ring_buff.sink_pos);
}

void it_doesnt_read_past_source() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, buff, len);
    
    ring_write(&ring_buff, 0x55);
    int read = ring_read(&ring_buff);
    CU_ASSERT(read == 0x55);
    read = ring_read(&ring_buff);
    CU_ASSERT(read == -1);
    CU_ASSERT(ring_buff.source_pos == ring_buff.sink_pos);
}

void it_writes_source_after_being_blocked() {
    unsigned char buff[4];
    int len = 4;
    RingBuff_t ring_buff;
    ring_init(&ring_buff, buff, len);
    
    ring_write(&ring_buff, 0x00);
    ring_write(&ring_buff, 0x01);
    ring_write(&ring_buff, 0x02);
    ring_write(&ring_buff, 0x03);
    int wrote = ring_write(&ring_buff, 0x04);
    CU_ASSERT(wrote == 0);
    CU_ASSERT(ring_buff.source_pos == ring_buff.sink_pos);
    int read = ring_read(&ring_buff);
    CU_ASSERT(ring_buff.source_pos == (ring_buff.sink_pos - 1));
    wrote = ring_write(&ring_buff, 0x04);
    CU_ASSERT(wrote == 1);
    CU_ASSERT(ring_buff.source_pos == ring_buff.sink_pos);
    CU_ASSERT(ring_buff.sink_iteration == (ring_buff.source_iteration - 1));
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
            (NULL == CU_add_test(pSuite, "it_writes_one", it_sources_one)) ||
            (NULL == CU_add_test(pSuite, "it_sink_one", it_sinks_one)) ||
            (NULL == CU_add_test(pSuite, "it_iterates_source", it_iterates_source)) ||
            (NULL == CU_add_test(pSuite, "it_iterates_sink", it_iterates_sink)) ||
            (NULL == CU_add_test(pSuite, "it_doesnt_write_past_sink_after_source_iterates", it_doesnt_write_past_sink_after_source_iterates)) ||
            (NULL == CU_add_test(pSuite, "it_doesnt_read_past_source", it_doesnt_read_past_source)) ||
            (NULL == CU_add_test(pSuite, "it_writes_source_after_being_blocked", it_writes_source_after_being_blocked)) ||
            (NULL == CU_add_test(pSuite, "it_inits_empty", it_inits_empty)) ||
            (NULL == CU_add_test(pSuite, "it_emtpies_after_sinked_bytes_read", it_emtpies_after_sinked_bytes_read)) || 
            (NULL == CU_add_test(pSuite, "it_emtpies_after_sinked_bytes_read_after_iteration", it_emtpies_after_sinked_bytes_read_after_iteration))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
