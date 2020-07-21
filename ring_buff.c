/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ring_buff.h"

int ring_init(RingBuff_t *ring_buff, unsigned char *buff, int len) {
    ring_buff->buff = buff;
    ring_buff->len = len;
    ring_buff->sink_pos = 0;
    ring_buff->source_pos = 0;
    ring_buff->sink_iteration = 0;
    ring_buff->source_iteration = 0;    
}

int ring_is_emtpy(RingBuff_t *ring_buff) {
    return ((ring_buff->sink_iteration == ring_buff->source_iteration) && 
            (ring_buff->sink_pos == ring_buff->source_pos));
}

int ring_read(RingBuff_t *ring_buff) {
    int next_ring_sink_pos;
    unsigned char next_ring_sink_iteration;
    
    unsigned char val;
    
    if(ring_buff->sink_pos < (ring_buff->len -1)) {
        next_ring_sink_pos = ring_buff->sink_pos + 1;
	next_ring_sink_iteration = ring_buff->sink_iteration;
    } else { 
        next_ring_sink_pos = 0;
        next_ring_sink_iteration = ring_buff->sink_iteration + 1;
    }
    
    if(
            ((next_ring_sink_pos <= ring_buff->source_pos) && (ring_buff->sink_iteration == ring_buff->source_iteration)) ||
            (ring_buff->sink_iteration != ring_buff->source_iteration)
            ) {
        val = ring_buff->buff[ring_buff->sink_pos]; 
        ring_buff->sink_pos = next_ring_sink_pos;
        ring_buff->sink_iteration = next_ring_sink_iteration;
    } else 
        return -1;

    return val;
}

int ring_write(RingBuff_t *ring_buff, unsigned char b) {
    if((ring_buff->source_pos == ring_buff->sink_pos) && (ring_buff->sink_iteration != ring_buff->source_iteration))
        return 0;
    
    ring_buff->buff[ring_buff->source_pos] = b;
    
    if(ring_buff->source_pos < (ring_buff->len -1)) {        
        ring_buff->source_pos++;
    } else {
        ring_buff->source_pos = 0;
        ring_buff->source_iteration++;
    }
    
    return 1;
}