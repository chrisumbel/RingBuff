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

#include "ring_buff.h"

/*
 * Initializes the RingBuff_t structure, which represents an instance of
 * a ring buffer. This expects the memory management of buff, the actual 
 * array of buffer data, to be handled by the caller. This is in anticipation
 * of use in environments, such as microcontrollers, which are memory-
 * constrained, and may not have malloc 
 */
int ring_init(RingBuff_t *ring_buff, unsigned char *buff, int len) {
    ring_buff->buff = buff;
    ring_buff->len = len;
    ring_buff->sink_pos = 0;
    ring_buff->source_pos = 0;
    ring_buff->sink_iteration = 0;
    ring_buff->source_iteration = 0;    
}

/*
 * Returns true if the buffer is empty or drained
 */
int ring_is_emtpy(RingBuff_t *ring_buff) {
    return ((ring_buff->sink_iteration == ring_buff->source_iteration) && 
            (ring_buff->sink_pos == ring_buff->source_pos));
}

/*
 * Reads a byte from the buffer to the sink. Returns unsigned char for the next 
 * byte in the buffer or -1 if the buffer is empty drained.
 */
int ring_read(RingBuff_t *ring_buff) {
    int next_ring_sink_pos;
    unsigned char next_ring_sink_iteration;
    
    unsigned char val;
    
    // Will I reach the upper bounds of the buffer?
    if(ring_buff->sink_pos < (ring_buff->len -1)) {
        // No, I'll keep moving forward
        next_ring_sink_pos = ring_buff->sink_pos + 1;
	next_ring_sink_iteration = ring_buff->sink_iteration;
    } else { 
        // Yes, I'll return to the beginning 
        next_ring_sink_pos = 0;
        next_ring_sink_iteration = ring_buff->sink_iteration + 1;
    }
    
    // Would I read past the writer? 
    if(
            ((next_ring_sink_pos <= ring_buff->source_pos) && (ring_buff->sink_iteration == ring_buff->source_iteration)) ||
            (ring_buff->sink_iteration != ring_buff->source_iteration)
            ) {
        // No, I can just read. There's data for me.
        val = ring_buff->buff[ring_buff->sink_pos]; 
        ring_buff->sink_pos = next_ring_sink_pos;
        ring_buff->sink_iteration = next_ring_sink_iteration;
    } else { 
        // Yes, don't read and return an error. The buffer is empty
        return -1;
    }
    
    return val;
}

/*
 * Write a byte from the source into the buffer
 */
int ring_write(RingBuff_t *ring_buff, unsigned char b) {
    // Would I write over bytes the reader hasn't read yet (am I full)?
    if((ring_buff->source_pos == ring_buff->sink_pos) && (ring_buff->sink_iteration != ring_buff->source_iteration)) {
        // Yes, don't write
        return 0;
    }
    
    // read a byte
    ring_buff->buff[ring_buff->source_pos] = b;
    
    // Would I overflow the buffer?
    if(ring_buff->source_pos < (ring_buff->len -1)) {        
        // No, keep moving forward
        ring_buff->source_pos++;
    } else {
        // Yes, go back to the beginning
        ring_buff->source_pos = 0;
        ring_buff->source_iteration++;
    }
    
    return 1;
}