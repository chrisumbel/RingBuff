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

#ifndef RING_BUFF_H
#define RING_BUFF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RingBuff {
    unsigned char *buff;
    int len;
    unsigned int sink_pos;
    unsigned int source_pos;
    unsigned char sink_iteration;
    unsigned char source_iteration;
} RingBuff_t;    

// Initializes the RingBuff_t structure, which represents an instance of a ring buffer.
int ring_init(RingBuff_t *ring_buff, unsigned char *buff, int len);
// Returns true if the buffer is empty or drained
int ring_is_emtpy(RingBuff_t *ring_buff);
// Reads a byte from the buffer to the sink.
int ring_read(RingBuff_t *ring_buff);
// Write a byte from the source into the buffer
int ring_write(RingBuff_t *ring_buff, unsigned char b);

#ifdef __cplusplus
}
#endif

#endif /* RING_BUFF_H */

